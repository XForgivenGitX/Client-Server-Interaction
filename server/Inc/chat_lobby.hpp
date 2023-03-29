#pragma once
#include <server_module.hpp>
namespace server
{
    struct simple_lobby :   public lobby, 
                            public boost::noncopyable, 
                            public std::enable_shared_from_this<simple_lobby>
    {
    
        db::lobby_database lobbyDataBase_{};
    
    private:
        db::server_database *serverDataBase_;

    public:
        simple_lobby(db::server_database *);
        void authorization(anet::socket_data_ptr socketData) override;
        void main_menu_response(anet::socket_data_ptr socketData, common::command) override;
        
    private:
        void auth_request_handler(anet::socket_data_ptr socketData, const boost::system::error_code &error) noexcept;
        void auth_responce_handler(anet::socket_data_ptr socketData, const boost::system::error_code &error) noexcept;
        void main_menu_request_handler(anet::socket_data_ptr socketData, const boost::system::error_code &error) noexcept;
        void main_menu_responce_handler(anet::socket_data_ptr socketData, const boost::system::error_code &error) noexcept;
    
    public:
        void leave_unauth_user(anet::socket_data_ptr socketData);
        void leave_auth_user(anet::socket_data_ptr socketData) override;
        void leave_all_users() override;
    };
    typedef std::shared_ptr<simple_lobby> simple_chat_lobby_ptr;
}