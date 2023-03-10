#include <client_handlers.hpp>

void client::send_handler(anet::socket_data_ptr&& socketData, const boost::system::error_code& error)
{
    std::cout << "Server received: " << socketData->data_ << std::endl;
    anet::send_receive::receive(std::move(socketData), std::function(client::receive_handler), 8, 2);
}

void client::receive_handler(anet::socket_data_ptr&& socketData, const boost::system::error_code& error)
{
    std::cout << "Server send: " << socketData->data_ << std::endl;
    system(socketData->data_.c_str());
    anet::send_receive::receive(std::move(socketData), std::function(client::receive_handler), 50, 2);
}

void client::connection_handler(anet::socket_data_endpoint_ptr &&socketData, const boost::system::error_code &)
{
   socketData->socketData_->data_ = std::string("Johnnn");
   anet::send_receive::send(std::move(socketData->socketData_), std::function(client::receive_handler));
}