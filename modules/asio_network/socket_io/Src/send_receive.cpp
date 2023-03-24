#include "send_receive.hpp"

namespace anet
{
void send_receive::send(socket_data_ptr socketData, callback_func_t &&handler)
{
    io__::async_write
        (
            socketData->socket_, io__::buffer(socketData->send_buffer_), 
            callback_function_wrapper(socketData, 
                std::move(handler), called_function(&send_called_function))
        );
}

void send_receive::receive(socket_data_ptr socketData, 
                    callback_func_t &&handler, std::size_t atMostBytes, std::size_t atLeastBytes)
{
    socketData->receive_buffer_.resize(atMostBytes);
    io__::async_read
        (
            socketData->socket_, io__::buffer(socketData->receive_buffer_), 
            io__::transfer_at_least(atLeastBytes), 
            callback_function_wrapper(socketData, 
                std::move(handler), called_function(&receive_called_function))
        );
}

void send_receive::send_called_function(const error_code &error, 
                                    std::size_t bytesTransferred, callback_function_wrapper& wrapper)
{
#ifdef NETWORK_ENABLE_HANDLER_TRACKING
    std::cout << "@>>: " << bytesTransferred << " bytes: " << socketData_->receive_buffer_ << '\n';
#endif
    wrapper.functionWrapped_(wrapper.socketData_, error);
}

void send_receive::receive_called_function(const error_code &error, 
                                    std::size_t bytesTransferred, callback_function_wrapper& wrapper)
{
    wrapper.socketData_->receive_buffer_.resize(bytesTransferred);
#ifdef NETWORK_ENABLE_HANDLER_TRACKING
    std::cout << "@>>: " << bytesTransferred << " bytes: " << socketData_->receive_buffer_ << '\n';
#endif
    wrapper.functionWrapped_(wrapper.socketData_, error);
}

send_receive::callback_function_wrapper::callback_function_wrapper
                        (socket_data_ptr& socketData, callback_func_t &&handler, called_function&& call)
    : socketData_(socketData), functionWrapped_(std::move(handler)), call_(std::move(call))
{}

void send_receive::callback_function_wrapper::operator()
                                (const error_code &error, std::size_t bytesTransferred)
{
    call_(error, bytesTransferred, *this);
}

}
