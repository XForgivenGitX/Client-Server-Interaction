#include <network_module.hpp>

void anet::send_receive::send(anet::socket_data_ptr socketData, callback_func_t &&handler)
{
    io__::async_write
        (
            socketData->socket_, io__::buffer(socketData->send_buffer_), 
            callback_function_wrapper(socketData, 
                std::move(handler), called_function(&send_called_function))
        );
}

void anet::send_receive::receive(anet::socket_data_ptr socketData, 
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

void anet::send_receive::send_called_function(const boost::system::error_code &error, 
                                    std::size_t bytesTransferred, callback_function_wrapper& wrapper)
{
#ifdef NETWORK_ENABLE_HANDLER_TRACKING
    std::cout << "@>>: " << bytesTransferred << " bytes: " << socketData_->receive_buffer_ << '\n';
#endif
    wrapper.functionWrapped_(wrapper.socketData_, error);
}

void anet::send_receive::receive_called_function(const boost::system::error_code &error, 
                                    std::size_t bytesTransferred, callback_function_wrapper& wrapper)
{
    wrapper.socketData_->receive_buffer_.resize(bytesTransferred);
#ifdef NETWORK_ENABLE_HANDLER_TRACKING
    std::cout << "@>>: " << bytesTransferred << " bytes: " << socketData_->receive_buffer_ << '\n';
#endif
    wrapper.functionWrapped_(wrapper.socketData_, error);
}

anet::send_receive::callback_function_wrapper::callback_function_wrapper
                                (anet::socket_data_ptr& socketData, callback_func_t &&handler, called_function&& call)
    : socketData_(socketData), functionWrapped_(std::move(handler)), call_(std::move(call))
{}

void anet::send_receive::callback_function_wrapper::operator()
                                (const boost::system::error_code &error, std::size_t bytesTransferred)
{
    call_(error, bytesTransferred, *this);
}
