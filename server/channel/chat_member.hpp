#pragma once
#include "interface.hpp"
#include "send_receive.hpp"

namespace server
{
    struct chat_member : 
        public IChat_member,
        public std::enable_shared_from_this<chat_member>
    {
        IConnection_ptr connectManager_;
        
        chat_member(const common::name_t &name, const common::pass_t &pass, IConnection_ptr connectManager)
            : IChat_member(name, pass), connectManager_(connectManager)  {}

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
                connectManager_->leave_user(socketData);
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
                connectManager_->leave_user(socketData);
                return;
            }
            switch (splitedPack.get_command())
            {
            case command::CREATE_ROOM:
            {
                auto name = splitedPack.get_argument(protocol::ROOM_NAME_INDEX);
                if(connectManager_->add_channel(name))
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
                if(!connectManager_->join_channel(name, socketData, shared_from_this()))
                {
                    send_command(socketData, command::ERROR_JOIN_ROOM);
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