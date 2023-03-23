#include <server_module.hpp>
server::chat_room::chat_room(const lobby_ptr &myLobby)
    : myLobby_(myLobby) {}

void server::chat_room::enter_member(const anet::socket_data_ptr &socketData, group_chat_member_ptr &&newMember)
{
    auto temp = &*newMember;
    mutex.lock();
    activeMembers_.insert(std::make_pair(socketData, std::move(newMember)));
    mutex.unlock();
    temp->start_communication(socketData);
}

void server::chat_room::send_all_members(const std::string &msg)
{
    std::shared_lock lock(mutex);
    auto assembledMsg = common::assemble_frame(common::command::SEND_MESSAGE, {msg});
    for (auto &[socketData, member] : activeMembers_)
    {
        socketData->send_buffer_ = assembledMsg; 
        anet::send_receive::send(socketData, {&group_chat_member::send_message_handler, &*member});
    }
}

void server::chat_room::leave_member(anet::socket_data_ptr socketData, std::string&& lastMsg)
{
    mutex.lock();
    activeMembers_.erase(socketData);
    mutex.unlock();
    send_all_members(lastMsg);
}

server::simple_group_chat_member::simple_group_chat_member(const chat_room_ptr &myRoom, const name_t &name)
    : myRoom_(myRoom), name_(name) {}

void server::simple_group_chat_member::start_communication(anet::socket_data_ptr socketData)
{
    socketData->send_buffer_ = assemble_frame(command::SUCCESS_JOIN_ROOM_RESP);
    anet::send_receive::send(socketData, {&simple_group_chat_member::start_communication_handler, this});

    std::string message("new member: ");
    myRoom_->send_all_members(message + name_);
}

void server::simple_group_chat_member::start_communication_handler(anet::socket_data_ptr& socketData, const boost::system::error_code &error) noexcept
{
    if (error)
    {
        forced_leave(socketData);
        return;
    }
    else
    {
        anet::send_receive::receive(socketData, {&simple_group_chat_member::receive_message_handler, this});
    }
}

void server::simple_group_chat_member::receive_message_handler(anet::socket_data_ptr& socketData, const boost::system::error_code &error) noexcept
{
    auto splited_frame = parse_and_chek_errors(socketData->receive_buffer_, error);
    if (!splited_frame)
    {
        forced_leave(socketData);
        return;
    }

    auto &[cmd, args] = splited_frame.value();
    switch (cmd)
    {
    case command::SEND_MESSAGE:
        socketData->receive_buffer_ = configure_message(args[0]);
        myRoom_->send_all_members(socketData->receive_buffer_);
        anet::send_receive::receive(socketData, {&simple_group_chat_member::receive_message_handler, this});
        break;

    case command::DETACH_ROOM_REQ:
        leave_room(socketData);
        break;
    }
}

void server::simple_group_chat_member::forced_leave(anet::socket_data_ptr socketData)
{
    myRoom_->myLobby_->leave_auth_user(socketData);
    myRoom_->leave_member(socketData, name_ + std::string(" left the chat due to a bad connection."));  
}

void server::simple_group_chat_member::leave_room(anet::socket_data_ptr socketData)
{
    myRoom_->myLobby_->main_menu_response(socketData, common::command::DETACH_ROOM_RESP);
    myRoom_->leave_member(socketData, name_ + std::string(" left the chat."));  
}

std::string server::simple_group_chat_member::configure_message(const std::string &usersMessage) const
{
    auto datetime = boost::posix_time::second_clock::universal_time();
    std::ostringstream configuredMessage;
    configuredMessage << "[" << datetime << "] " << name_ << ": " << usersMessage;
    return configuredMessage.str();
}

void server::simple_group_chat_member::send_message_handler(anet::socket_data_ptr& socketData, const boost::system::error_code &error) noexcept
{
    if (error)
    {
        forced_leave(socketData);
        return;
    }
}