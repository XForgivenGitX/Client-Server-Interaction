#include <network_module.hpp>

void anet::send_receive::send(anet::socket_data_ptr socketData, callback_func_t &&handler)
{
    if(!socketData->socket_.is_open()) throw utility::bad_socket{};
    auto &[socket, send_buffer, receive_buffer] = *socketData;
    io__::async_write(socket, io__::buffer(send_buffer), callback_function_wrapper(socketData, std::move(handler), tags::send_tag));
}

void anet::send_receive::receive(anet::socket_data_ptr socketData, callback_func_t &&handler, std::size_t atMostBytes, std::size_t atLeastBytes)
{
    if(!socketData->socket_.is_open()) throw utility::bad_socket{};
    auto &[socket, send_buffer, receive_buffer] = *socketData;
    receive_buffer.resize(atMostBytes);
    io__::async_read(socket, io__::buffer(receive_buffer), io__::transfer_at_least(FRAME_MIN_LENGHT), callback_function_wrapper(socketData, std::move(handler), tags::receive_tag));
    
}

anet::send_receive::callback_function_wrapper::callback_function_wrapper
                                (anet::socket_data_ptr socketData, callback_func_t &&handler, tags tag)
    : socketData_(socketData), functionWrapped_(std::move(handler)), tag_(tag)
{}

void anet::send_receive::callback_function_wrapper::operator()
                                (const boost::system::error_code &error, std::size_t bytesTransferred) noexcept
{
    if(tag_ == tags::receive_tag)
            socketData_->receive_buffer_.resize(bytesTransferred);

#ifdef NETWORK_MODULE_DEBUG
    switch(tag_)
    {
        case tags::receive_tag:
            std::cout << "Receive: " << bytesTransferred << " bytes, " << socketData_->receive_buffer_ << '\n';
            break;
        case tags::send_tag:
            std::cout << "Send: " << bytesTransferred << " bytes, " << socketData_->send_buffer_ << '\n';
            break;
    }
#endif
    
    utility::task_wrapped<void()>([this, error]{functionWrapped_.taskUnwrapped_(socketData_, error);})(); 
}