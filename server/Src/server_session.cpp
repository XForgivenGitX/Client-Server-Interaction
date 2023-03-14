#include <server_module.hpp>

server::server_session::server_session(io__::io_context &ios, unsigned short port)
    : ios_(ios), endPoint_(port, io__::ip::tcp::v4()) {}

void server::server_session::start()
{
    accept_connections(utility::safe_make_unique<anet::tcp_listener>(ios_, endPoint_));
    std::cout << "Server was started on " << endPoint_.point_.port() << " port.\n";
}

void server::server_session::accepted_connection_handler(anet::tcp_listener_ptr &&listener, const boost::system::error_code &error) noexcept
{
    auto newSocket = std::move(listener->socketData_);
    listener->socketData_ = utility::safe_make_shared<anet::socket_data>(ios_);
    if (error)
    {
        std::cerr << "Connection error, system:" << error.message() << ".\n";
        newSocket->shutdown();
    }
    else
    { 
#ifdef SERVER_MODULE_DEBUG
        std::cout << "Successful connection.\nSystem:"
                  << error.message()
                  << "\nIPv4: "
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
    error ? socketData->shutdown() : room_.enter_user(socketData, utility::safe_make_shared<simple_chat_user>(room_, socketData->receive_buffer_));
}