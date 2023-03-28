#pragma once
#include <memory>
#include <algorithm>
#include "send_receive.hpp"
#include "i_channels.hpp"
#include "user_account.hpp"
#include "protocol.hpp"
#include "logger.hpp"

namespace server
{
    struct main_lobby :    
        public ILobby, 
        public boost::noncopyable, 
        public std::enable_shared_from_this<main_lobby>
    {
        using cmd_type = common::command;

    public:
        void enter_user(anet::socket_data_ptr&) override;
        
        void leave_user(anet::socket_data_ptr&) override;
        void leave_all() override
        {

        }

    private:
        void send_command(anet::socket_data_ptr&, cmd_type cmd);
        void send_command_handler(anet::socket_data_ptr&, const anet::err_c& error);
        void receive_command_handler(anet::socket_data_ptr& , const anet::err_c& error);
    };
    typedef std::shared_ptr<main_lobby> main_lobby_ptr;
}