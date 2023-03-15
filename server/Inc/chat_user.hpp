#pragma once
#include <server_module.hpp>

namespace server
{
    class simple_chat_user :    public chat_user, 
                                public boost::noncopyable, 
                                public std::enable_shared_from_this<simple_chat_user>
    {   
        chat_room& myRoom_;
    
    public:
        data_t name_;
        
    public:
        simple_chat_user(chat_room& myRoom, const data_t& name);
 
    private:
        std::string configure_message(const std::string& usersMessage) const;
    
    public:
        void receive_message_handler(anet::socket_data_ptr socketData, const boost::system::error_code& error) noexcept override;
        void send_message_handler(anet::socket_data_ptr socketData, const boost::system::error_code& error) noexcept override;
    };
}
