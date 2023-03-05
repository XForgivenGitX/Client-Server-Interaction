#include <server_handlers.hpp>

void server::send_handler(anet::socket_data_ptr&& socketData, const boost::system::error_code& error)
{
    std::cout << "Client received: " << socketData->data_ << std::endl;
    anet::send_receive::receive(std::move(socketData), 8, std::function(receive_handler));
}

void server::receive_handler(anet::socket_data_ptr&& socketData, const boost::system::error_code& error)
{
    std::cout << "Client send: " << socketData->data_ << std::endl;
    anet::send_receive::send(std::move(socketData), std::function(send_handler));
}

void server::handler_accepted_connection(anet::tcp_listener_ptr&& listener, const boost::system::error_code& error)
{
    std::cout << "Client connected!" << std::endl;
    anet::send_receive::receive(std::move(listener->socketData_), 8, std::function(receive_handler));
}