// #include <server_module.hpp>

// void server::chat_room::enter_user(const anet::socket_data_ptr& socketData, const group_chat_member_ptr& newMember)
// {
//     mutex.lock();
//     activeMembers_.insert(std::make_pair(socketData, newMember));
//     mutex.unlock();
//     anet::send_receive::receive(socketData, {&group_chat_member::receive_message_handler, &*newMember});

// }

// void server::chat_room::leave_user(const anet::socket_data_ptr& user)
// {
//     std::lock_guard lock(mutex);
//     activeMembers_.erase(user);
// }

// void server::chat_room::send_all(const std::string& msg)
// {
//     std::lock_guard lock(mutex);
//     msgHistory.push_back(msg);
//     for(auto& [socketData, client] : activeMembers_)
//     {
//         socketData->send_buffer_ = msg;
//         anet::send_receive::send(socketData, {&group_chat_member::send_message_handler, &*client}); 
//     }
// }

// void server::chat_room::send_history(const anet::socket_data_ptr& socketData, const group_chat_member_ptr& user)
// {
//     for(auto& msg : msgHistory)
//     {   
//         socketData->send_buffer_ = msg;
//         anet::send_receive::send(socketData, {&group_chat_member::send_message_handler, &*user}); 
//     }
// }

// server::simple_group_chat_member::simple_group_chat_member(const chat_room_ptr& myRoom, const name_t & name)
//     : myRoom_(myRoom), name_(name)
// {
//     std::string message("new member: ");
//     myRoom_->send_all(message + name_);
// }

// std::string server::simple_group_chat_member::configure_message(const std::string& usersMessage) const
// {
//     auto datetime = boost::posix_time::second_clock::universal_time();
//     std::ostringstream configuredMessage;
//     configuredMessage << "[" << datetime << "] " << name_ << ": " << usersMessage;
//     return configuredMessage.str();
// }

// void server::simple_group_chat_member::receive_message_handler(anet::socket_data_ptr socketData, const boost::system::error_code &error) noexcept
// {
//     if(error) 
//     { 
//         std::string message(name_ + std::string(" left the room"));
//         myRoom_->leave_user(socketData);
//         myRoom_->send_all(message);
//         return;
//     }
//     else
//     {
//         myRoom_->send_all(configure_message(socketData->receive_buffer_));
//         anet::send_receive::receive(socketData, {&simple_group_chat_member::receive_message_handler, shared_from_this()});
//     } 
// }

// void server::simple_group_chat_member::send_message_handler(anet::socket_data_ptr socketData, const boost::system::error_code &error) noexcept
// {

// }