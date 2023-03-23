#include <network_module.hpp>

void anet::connection::connection_request(socket_data_ptr socketData, const end_point_wrapper &endPoint, callback_func_t &&handler)
{
    boost::system::error_code error;
    socketData->socket_.connect(endPoint.point_, error);
    handler(socketData, error);
}

void anet::connection::async_connection_request(const io__::any_io_executor& ios, socket_data_ptr socketData, const end_point_wrapper &endPoint, callback_func_t &&handler)
{
    boost::system::error_code error;
    socketData->socket_.connect(endPoint.point_, error);
    io__::post(ios, [handler, socketData_ = std::move(socketData), error]() mutable
    {handler(socketData_, error);});
}

anet::tcp_listener::tcp_listener(const io__::any_io_executor& ios, const end_point_wrapper &endPoint)
    : acceptor_(ios, endPoint.point_), socketData_(std::make_unique<socket_data>(ios))
{
}

anet::listen::callback_function_wrapper::callback_function_wrapper(
                                tcp_listener_ptr &&listener, callback_func_t &&handler)
    : listener_(std::move(listener)), functionUnwrapped_(std::move(handler))
{
}

void anet::listen::callback_function_wrapper::operator()(const boost::system::error_code &error) noexcept
{
    functionUnwrapped_(std::move(listener_), error);
}

void anet::listen::accepting_connection(tcp_listener_ptr&& listener, callback_func_t&& handler)
{
    if(!listener->acceptor_.is_open()) throw utility::bad_socket{};
    auto& [acceptor, socketData] = *listener;
    acceptor.async_accept(socketData->socket_, 
                callback_function_wrapper(std::move(listener), std::move(handler)));
}