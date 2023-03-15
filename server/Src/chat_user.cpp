#include <server_module.hpp>
#include "chat_user.hpp"

server::simple_chat_user::simple_chat_user(chat_room& myRoom, const data_t &name)
    : myRoom_(myRoom), name_(name)
{
    std::string message("New member: ");
    myRoom_.send_all(message + name_);
}

std::string server::simple_chat_user::configure_message(const std::string& usersMessage) const
{
    auto datetime = boost::posix_time::second_clock::universal_time();
    std::ostringstream configuredMessage;
    configuredMessage << "[" << datetime << "] " << name_ << ": " << usersMessage;
    return configuredMessage.str();
}

void server::simple_chat_user::receive_message_handler(anet::socket_data_ptr socketData, const boost::system::error_code &error) noexcept
{
#ifdef SERVER_MODULE_DEBUG
    std::cout << ">> " << socketData->receive_buffer_ << std::endl;
#endif
    if(error) 
    { 
        std::string message(name_ + std::string(" left the chat."));
        myRoom_.leave_user(socketData);
        myRoom_.send_all(message);
        return;
    }
    else
    {
        myRoom_.send_all(configure_message(socketData->receive_buffer_));
        anet::send_receive::receive(socketData, {simple_chat_user::receive_message_handler, shared_from_this()});
    } 
}

void server::simple_chat_user::send_message_handler(anet::socket_data_ptr socketData, const boost::system::error_code &error) noexcept
{
#ifdef SERVER_MODULE_DEBUG
    std::cout << "<< " << socketData->send_buffer_ << '\n';
#endif
}