#pragma once
#include <server_module.hpp>

namespace server
{
    struct chat_user
    {
        virtual void receive_message_handler(anet::socket_data_ptr socketData, const boost::system::error_code &error) = 0;
        virtual void send_message_handler(anet::socket_data_ptr socketData, const boost::system::error_code &error) = 0;
        virtual ~chat_user() = default;
    };
    typedef std::shared_ptr<chat_user> chat_user_ptr;
    
    struct chat_room : boost::noncopyable
    {
        using clients = std::unordered_map<anet::socket_data_ptr, chat_user_ptr>;
    
    private:
        clients connectedUsers_;
        mutable std::shared_mutex mutex;

    public:
        void enter_user(anet::socket_data_ptr socketData, chat_user_ptr newUser);
        void leave_user(anet::socket_data_ptr user);
        void send_all(std::string message) const;
    };
}