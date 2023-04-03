#include "send_receive.hpp"

namespace anet
{
void send_receive::send(const socket_data_ptr& socketData, callback_func_t &&handler)
{
    io__::async_write
        (
            socketData->socket_, io__::buffer(socketData->send_buffer_), 
            callback_function_wrapper(socketData, 
                std::move(handler), called_function(&send_called_function))
        );
}

void send_receive::receive(const socket_data_ptr& socketData, 
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

void send_receive::send_called_function(const err_c &error_c, 
                                [[maybe_unused]] std::size_t bytesTransferred, callback_function_wrapper* wrapper)
{
#ifdef NETWORK_ENABLE_HANDLER_TRACKING
    BOOST_LOG_TRIVIAL(info)
            << lg::build_src("send package")
                << lg::build_arg("sock: " + std::to_string(wrapper->socketData_->get_handle()))
                << lg::build_arg("bytes: " + std::to_string(bytesTransferred))
                << lg::build_arg("pack: " + wrapper->socketData_->send_buffer_) 
                << lg::build_arg("status: " + error_c.message());
    
#endif
    wrapper->functionWrapped_(wrapper->socketData_, error_c);
}

void send_receive::receive_called_function(const err_c &error_c, 
                                [[maybe_unused]] std::size_t bytesTransferred, callback_function_wrapper* wrapper)
{
    wrapper->socketData_->receive_buffer_.resize(bytesTransferred);
#ifdef NETWORK_ENABLE_HANDLER_TRACKING
    BOOST_LOG_TRIVIAL(info)
            << lg::build_src("receive package")
                << lg::build_arg("sock: " + std::to_string(wrapper->socketData_->get_handle()))
                << lg::build_arg("bytes: " + std::to_string(bytesTransferred))
                << lg::build_arg("pack: " + wrapper->socketData_->receive_buffer_)
                << lg::build_arg("status: " + error_c.message());
#endif
    wrapper->functionWrapped_(wrapper->socketData_, error_c);
}

send_receive::callback_function_wrapper::callback_function_wrapper
                        (const socket_data_ptr& socketData, callback_func_t &&handler, called_function&& call)
    : socketData_(socketData), functionWrapped_(std::move(handler)), call_(std::move(call))
{}

void send_receive::callback_function_wrapper::operator()
                                (const err_c &error_c, std::size_t bytesTransferred)
{
    call_(error_c, bytesTransferred, this);
}

}
