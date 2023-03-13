#include <server_module.hpp>

void server::chat_room::enter_user(anet::socket_data_ptr socketData, chat_user_ptr newUser)
{
    mutex.lock();
    connectedUsers_[socketData] = newUser;
    mutex.unlock();
    anet::send_receive::receive(socketData, {chat_user::receive_message_handler, newUser});
}

void server::chat_room::leave_user(anet::socket_data_ptr user)
{
    std::lock_guard lock(mutex);
    connectedUsers_.erase(user);
}

void server::chat_room::send_all(std::string message) const
{
    std::shared_lock lock(mutex);
    std::for_each(connectedUsers_.begin(), connectedUsers_.end(), [&message](auto &user)
    {     
        auto& [socketData, client] = user;
        socketData->send_buffer_ = message;
        anet::send_receive::send(socketData, {chat_user::send_message_handler, client}); 
    });
}
