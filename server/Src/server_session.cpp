#include <server_module.hpp>
#if i
server::server_session::server_session(io__::io_context &ios, unsigned short port)
    : ios_(ios), endPoint_(port, io__::ip::tcp::v4()) {}

void server::server_session::start()
{
    accept_connections(std::make_unique<anet::tcp_listener>(ios_, endPoint_));
    std::cout << "Server was started on " << endPoint_.point_.port() << " port.\n";
}

void server::server_session::accepted_connection_handler(anet::tcp_listener_ptr &&listener, const boost::system::error_code &error) noexcept
{
    auto newSocket = std::move(listener->socketData_);
    listener->socketData_ = std::make_shared<anet::socket_data>(ios_);
    if (error)
    {
        std::cerr << "Connection error, system:" << error.message() << ".\n";
        newSocket->shutdown();
    }
    else
    {
#ifdef SERVER_MODULE_DEBUG
        std::cout << "Successful connection. System: "
                  << error.message()
                  << ". IPv4: "
                  << newSocket->socket_.remote_endpoint().address().to_string()
                  << '\n';
#endif
        anet::send_receive::receive(newSocket, {authorization_handler, this});
    }
    accept_connections(std::move(listener));
}

void server::server_session::accept_connections(anet::tcp_listener_ptr &&listener) noexcept
{
    anet::listen::accepting_connection(std::move(listener), {accepted_connection_handler, this});
}

void server::server_session::authorization_handler(anet::socket_data_ptr socketData, const boost::system::error_code &error) noexcept
{
    error ? socketData->shutdown() : room_.enter_user(socketData, std::make_shared<simple_chat_user>(room_, socketData->receive_buffer_));
}
#endif

void server::chatLobby::authorization(anet::socket_data_ptr socketData)
{
    if (dataBase.is_register_user(server_control_block::get_mutable_instance().connectedIP[socketData]))
        socketData->send_buffer_ = common::assemble_frame(command::LOG_IN_REQ);
    else
        socketData->send_buffer_ = common::assemble_frame(command::REGISTER_REQ);
    anet::send_receive::send(socketData, {auth_request_handler, this});
}

void server::chatLobby::auth_request_handler(anet::socket_data_ptr socketData, const boost::system::error_code &error) noexcept
{
    anet::send_receive::receive(socketData, {user_responce_handler, this});
}

void server::chatLobby::user_responce_handler(anet::socket_data_ptr socketData, const boost::system::error_code &error) noexcept
{
    auto resp = common::split_frame(socketData->receive_buffer_);
    if (error || !resp)
    {
        server_control_block::get_mutable_instance().connectedIP.erase(socketData);
        socketData->shutdown();
        return;
    }

    auto &[cmd, args] = resp.value();
    switch (cmd)
    {
    case command::REGISTER_RESP:
        if (!dataBase.is_contains_name(args[NAME_INDEX]))
        {
            dataBase.add_user(server_control_block::get_mutable_instance().connectedIP[socketData],
                              registered_user{args[NAME_INDEX], args[PASS_INDEX]});
            std::cout << "---the user has successfully registrate the lobby" << '\n';
            // вход в лобби
        }
        else
        {
            socketData->send_buffer_ = common::assemble_frame(command::REPEAT_REG_REQ);
            anet::send_receive::send(socketData, {auth_request_handler, this});
        }
        break;

    case command::LOG_IN_RESP:
        if (dataBase.is_valid_user_data(server_control_block::get_mutable_instance().connectedIP[socketData],
                                        args[NAME_INDEX], args[PASS_INDEX]))
        {
            // вход в лобби
            std::cout << "---the user has successfully logged into the lobby" << '\n';
        }
        else
        {
            socketData->send_buffer_ = common::assemble_frame(command::REPEAT_LOG_IN_REQ);
            anet::send_receive::send(socketData, {auth_request_handler, this});
        }
        break;

    default:
    {
        server_control_block::get_mutable_instance().connectedIP.erase(socketData);
        socketData->shutdown();
        return;
    }
    }
}