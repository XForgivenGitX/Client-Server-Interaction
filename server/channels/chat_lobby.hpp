#pragma once
#include <memory>
#include <algorithm>
#include "send_receive.hpp"
#include "i_channels.hpp"
#include "connections.hpp"
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
        //db::lobby_database lobbyDataBase_{};

    public:
        void enter_user(anet::socket_data_ptr&) override;
        void send_command(anet::socket_data_ptr&, cmd_type cmd) override;
        void leave_user(anet::socket_data_ptr&) override;
    
    private:
        void send_command_handler(anet::socket_data_ptr&, const anet::err_c& error);
        void receive_command_handler(anet::socket_data_ptr& , const anet::err_c& error);
        // void main_menu_response(anet::socket_data_ptr socketData, common::command) override;
        
    // private:
    //     void auth_request_handler(anet::socket_data_ptr& socketData, const boost::system::error_code &error) noexcept;
    //     void auth_responce_handler(anet::socket_data_ptr& socketData, const boost::system::error_code &error) noexcept;
    //     void main_menu_request_handler(anet::socket_data_ptr& socketData, const boost::system::error_code &error) noexcept;
    //     void main_menu_responce_handler(anet::socket_data_ptr& socketData, const boost::system::error_code &error) noexcept;
    
    // public:
    //     void leave_unauth_user(anet::socket_data_ptr socketData);
    //     void leave_auth_user(anet::socket_data_ptr socketData) override;
    //     void leave_all_users() override;
    };
    typedef std::shared_ptr<main_lobby> main_lobby_ptr;
}