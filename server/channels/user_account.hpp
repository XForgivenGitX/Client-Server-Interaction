#pragma once
#include "i_channels.hpp"
#include "send_receive.hpp"

namespace server
{
    struct user_account : public IUser
    {
        ILobby_ptr myLobby_;
        IChannel_ptr currentChannel_;
        
        user_account(const common::name_t &name, const common::pass_t &pass, ILobby_ptr myLobby)
            : IUser(name, pass), myLobby_(myLobby), currentChannel_(nullptr)  {}

    public:
        void send_command(anet::socket_data_ptr &socketData, common::command cmd) override
        {
            socketData->send_buffer_ = common::assemble_package(cmd);
            anet::send_receive::send(socketData, {send_command_handler, this});
        }

    private:
        void send_command_handler(anet::socket_data_ptr &socketData, const anet::err_c &error_c)
        {
#ifdef SERVER_ENABLE_HANDLER_TRACKING
            BOOST_LOG_TRIVIAL(info)
                << lg::build_log(lg::address_cat("user=", this),
                                 "called send command handler",
                                 "sock: " + std::to_string(socketData->get_handle()),
                                 "pack: " + socketData->send_buffer_,
                                 "status: " + error_c.message());
#endif
            if (error_c)
            {
                myLobby_->leave_user(socketData);
            }
            else
            {
                anet::send_receive::receive(socketData, {receive_command_handler, this});
            }
        }

        void receive_command_handler(anet::socket_data_ptr &socketData, const anet::err_c &error_c)
        {
            using namespace common;
            transf_package splitedPack;
            splitedPack.disassemble(socketData->receive_buffer_);
#ifdef SERVER_ENABLE_HANDLER_TRACKING
            BOOST_LOG_TRIVIAL(info)
                << lg::build_log(lg::address_cat("user=", this),
                    "called receive command handler",
                    "sock: " + std::to_string(socketData->get_handle()),
                    "pack: " + socketData->receive_buffer_,
                    "match: " + lg::boolalpha_cast(splitedPack.isMatched()),
                    "status: " + error_c.message());
#endif
            if (error_c || !splitedPack.isMatched())
            {
                myLobby_->leave_user(socketData);
                return;
            }
            switch (splitedPack.get_command())
            {
            case command::CREATE_ROOM:
            {
                auto name = splitedPack.get_argument(protocol::ROOM_NAME_INDEX);
                if(myLobby_->add_channel(name))
                {
                    send_command(socketData, command::SUCCESS_CREATE_ROOM);
                }
                else
                {
                    send_command(socketData, command::ERROR_CREATE_ROOM);
                }
            }
            break;

            case command::JOIN_ROOM:
            {
                auto name = splitedPack.get_argument(protocol::ROOM_NAME_INDEX);
                if(auto channelOpt = myLobby_->join_channel(name, socketData); channelOpt)
                {
                    currentChannel_ = channelOpt.value();
                    send_command(socketData, command::SUCCESS_JOIN_ROOM);
                }
                else
                {
                    send_command(socketData, command::ERROR_JOIN_ROOM);
                }
            }
            break;
            
            case command::SEND_MESSAGE:
            {
                auto msg = splitedPack.get_argument(protocol::MESSAGE_INDEX);
                if(currentChannel_ != nullptr)
                {
                    currentChannel_->send_all(msg);
                    anet::send_receive::receive(socketData, {receive_command_handler, this});
                }
            }
            break;

            default:
#ifdef SERVER_ENABLE_HANDLER_TRACKING
                BOOST_LOG_TRIVIAL(error)
                    << lg::build_log(lg::address_cat("user=", this),
                                     "unknown command accepted");
#endif
            break;
            }
        }
    };
}