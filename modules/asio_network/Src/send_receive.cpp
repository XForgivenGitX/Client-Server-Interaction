#include <network_module.hpp>

void anet::send_receive::send(anet::socket_data_ptr socketData, callback_func_t &&handler)
{
    if(!socketData->socket_.is_open()) throw utility::bad_socket{};
    auto &[socket, data] = *socketData;
    io__::async_write(socket, io__::buffer(data), callback_function_wrapper(socketData, std::move(handler)));
}

void anet::send_receive::receive(anet::socket_data_ptr socketData, callback_func_t &&handler, std::size_t atMostBytes, std::size_t atLeastBytes)
{
    if(!socketData->socket_.is_open()) throw utility::bad_socket{};
    socketData->data_.resize(atMostBytes);
    auto &[socket, data] = *socketData;
    io__::async_read(socket, io__::buffer(data), io__::transfer_at_least(atLeastBytes), callback_function_wrapper(socketData, std::move(handler)));
}

anet::send_receive::callback_function_wrapper::callback_function_wrapper
                                (anet::socket_data_ptr socketData, callback_func_t &&handler)
    : socketData_(socketData), functionWrapped_(std::move(handler))
{}

void anet::send_receive::callback_function_wrapper::operator()
                                (const boost::system::error_code &error, std::size_t bytesTransferred) noexcept
{
    socketData_->data_.resize(bytesTransferred);
    utility::task_wrapped(std::move(functionWrapped_))(socketData_, error);
}