#include <server_module.hpp>
using namespace common;

server::simple_lobby::simple_lobby(db::server_database* serverDataBase)
    : serverDataBase_(serverDataBase) {}

void server::simple_lobby::authorization(anet::socket_data_ptr socketData)
{
    auto userIp = socketData->socket_.remote_endpoint().address().to_string();
    lobbyDataBase_.insert_active_ip(socketData, userIp);
    if (serverDataBase_->is_register_user(userIp))
        socketData->send_buffer_ = common::assemble_frame(command::LOG_IN_REQ);
    else
        socketData->send_buffer_ = common::assemble_frame(command::REGISTER_REQ);
    anet::send_receive::send(socketData, {auth_request_handler, this});
}

void server::simple_lobby::main_menu_response(anet::socket_data_ptr socketData, common::command cmd)
{
    socketData->send_buffer_ = common::assemble_frame(cmd);
    anet::send_receive::send(socketData, {main_menu_responce_handler, this});
}

void server::simple_lobby::auth_request_handler(
    anet::socket_data_ptr socketData, const boost::system::error_code &error) noexcept
{
    if(error)
    { 
        leave_unauth_user(socketData); 
        return;
    }
    anet::send_receive::receive(socketData, {auth_responce_handler, this});
}

void server::simple_lobby::auth_responce_handler(
    anet::socket_data_ptr socketData, const boost::system::error_code &error) noexcept
{
    auto msg = parse_and_chek_errors(socketData->receive_buffer_, error);
    if (!msg)
    {
        leave_unauth_user(socketData);
        return;
    }
    auto &[cmd, args] = msg.value();
    auto &name = args[NAME_INDEX];
    auto &pass = args[PASS_INDEX];

    switch (cmd)
    {
    case command::REGISTER_RESP:
        if (!serverDataBase_->is_contains_user_name(name))
        {
            serverDataBase_->add_user(lobbyDataBase_.get_active_ip(socketData), {name, pass});
            lobbyDataBase_.insert_active_name(socketData, name);
            main_menu_response(socketData, command::SUCCESS_REG_RESP);
        }
        else
        {
            socketData->send_buffer_ = common::assemble_frame(command::ECHO_REGISTER_REQ);
            anet::send_receive::send(socketData, {auth_request_handler, this});
        }
        break;

    case command::LOG_IN_RESP:
        if (serverDataBase_->is_valid_user_data(lobbyDataBase_.get_active_ip(socketData),
                                                          args[NAME_INDEX], args[PASS_INDEX]))
        {
            lobbyDataBase_.insert_active_name(socketData, name);
            main_menu_response(socketData, command::SUCCESS_LOG_IN_RESP);
        }
        else
        {
            socketData->send_buffer_ = common::assemble_frame(command::ECHO_LOG_IN_REQ);
            anet::send_receive::send(socketData, {auth_request_handler, this});
        }
        break;

    default:
    {
        leave_unauth_user(socketData);
        return;
    }
    }
}
void server::simple_lobby::main_menu_responce_handler(
    anet::socket_data_ptr socketData, const boost::system::error_code &error) noexcept
{
    if(error)
    { 
        leave_auth_user(socketData); 
        return;
    }
    anet::send_receive::receive(socketData, {main_menu_request_handler, this});
}

void server::simple_lobby::main_menu_request_handler(
    anet::socket_data_ptr socketData, const boost::system::error_code &error) noexcept
{
    auto msg = parse_and_chek_errors(socketData->receive_buffer_, error);
    if (!msg)
    {
        leave_auth_user(socketData);
        return;
    }
    auto &[cmd, args] = msg.value();
    auto& name = args[NAME_INDEX];
    switch(cmd)
    {
        case command::CREATE_ROOM_REQ:
            if(!lobbyDataBase_.is_contains_room_name(name) && !serverDataBase_->is_contains_user_name(name))
            {
                auto newRoom = std::make_shared<server::chat_room>(shared_from_this());
                lobbyDataBase_.insert_room(newRoom, name);
                main_menu_response(socketData, command::SUCCESS_CREATE_ROOM_RESP);
            }
            else
            {
                main_menu_response(socketData, command::ERROR_CREATE_ROOM_RESP);
            }
            break;
        
        case command::JOIN_ROOM_REQ:
            if(lobbyDataBase_.is_contains_room_name(name))
            {
                auto room = lobbyDataBase_.get_chat_room_ptr(name);
                auto userName =  lobbyDataBase_.get_active_name(socketData);
                auto roomMember = std::make_unique<server::simple_group_chat_member>(room, userName);
                room->enter_member(socketData, std::move(roomMember));
            }
            else
            {
                main_menu_response(socketData, command::ERROR_JOIN_ROOM_RESP);
            }
            break;
    }


}

void server::simple_lobby::leave_unauth_user(anet::socket_data_ptr socketData)
{
    lobbyDataBase_.erase_active_ip(socketData);
    socketData->shutdown();
}

void server::simple_lobby::leave_auth_user(anet::socket_data_ptr socketData)
{
    lobbyDataBase_.erase_active_ip(socketData);
    lobbyDataBase_.erase_active_name(socketData);
    socketData->shutdown();
}
void server::simple_lobby::leave_all_users()
{
    lobbyDataBase_.shutdown_all_sockets();
}
