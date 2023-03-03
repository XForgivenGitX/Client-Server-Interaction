#include <ClientServer.hpp>

void sendReceive::Action::send(sendReceive::SocketDataPtr &&socketData, callback_func_t &&func)
{
    auto &[socket, data] = *socketData;
    io__::async_write(socket, io__::buffer(data), CallbackFunctionWrapper(std::move(socketData), std::move(func)));
}

void sendReceive::Action::receive(sendReceive::SocketDataPtr &&socketData, std::size_t at_least_bytes, callback_func_t &&func)
{
    socketData->data.resize(at_least_bytes);
    auto &[socket, data] = *socketData;
    io__::async_read(socket, io__::buffer(data), CallbackFunctionWrapper(std::move(socketData), std::move(func)));
}

sendReceive::CallbackFunctionWrapper::CallbackFunctionWrapper(sendReceive::SocketDataPtr &&socketData, callback_func_t &&func)
    : socketData_(std::move(socketData)), functionWrapped_(std::move(func))
{}

void sendReceive::CallbackFunctionWrapper::operator()(const boost::system::error_code &error, std::size_t bytes_transferred) noexcept
{
    socketData_->data.resize(bytes_transferred);
    task::make_task_wrapped(std::move(functionWrapped_))(std::move(socketData_), error);
}