#include <client_handlers.hpp>

void client::send_handler(anet::socket_data_ptr&& socketData, const boost::system::error_code& error)
{
    std::cout << "Server received: " << socketData->data_ << std::endl;
    anet::send_receive::receive(std::move(socketData), 8, std::function(client::receive_handler));
}

void client::receive_handler(anet::socket_data_ptr&& socketData, const boost::system::error_code& error)
{
    std::cout << "Server send: " << socketData->data_ << std::endl;
    std::cout << "Enter your message (8 bytes):\n";
    std::cin >> socketData->data_;
    anet::send_receive::send(std::move(socketData), std::function(client::send_handler));
}

void client::connection_handler(anet::socket_data_endpoint_ptr &&socketData, const boost::system::error_code &)
{
    std::cout << "Connect succsessful, enter your message(8 byte):\n";
    std::cin >> socketData->socketData_->data_;
    anet::send_receive::send(std::move(socketData->socketData_), std::function(send_handler));
}