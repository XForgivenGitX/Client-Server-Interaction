#pragma once
#include "i_channels.hpp"
#include "send_receive.hpp"

namespace server
{
    struct user_account : public IUser
    {
        IChannel_ptr currentChannel_;
        ILobby_ptr myLobby_;
        user_account(const common::name_t& name, const common::pass_t& pass, ILobby_ptr myLobby)
            : IUser(name, pass), myLobby_(myLobby) {}
        
    public:
        void send_command(anet::socket_data_ptr& socketData, common::command cmd) override
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