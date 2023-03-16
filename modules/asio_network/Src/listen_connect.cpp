#include <network_module.hpp>

void anet::connection::connection_request(socket_data_endpoint_ptr &&socketDataEndpoint, callback_func_t &&handler)
{
    boost::system::error_code error = socketDataEndpoint->connect();
    handler(std::move(socketDataEndpoint), error);
}

void anet::connection::async_connection_request(io__::io_context &ios, socket_data_endpoint_ptr &&socketDataEndpoint, callback_func_t &&handler)
{
    boost::system::error_code error = socketDataEndpoint->connect();
    callback_func_t wrappedHandler(std::move(handler));
    io__::post(ios, [wrappedHandler, socketDataEndpoint_ = std::move(socketDataEndpoint), error]() mutable
    {wrappedHandler(std::move(socketDataEndpoint_), error);});
}

anet::tcp_listener::tcp_listener(io__::io_context &ios, const end_point_wrapper &endPoint)
    : acceptor_(ios, endPoint.point_), socketData_(std::make_unique<socket_data>(ios))
{
}
anet::tcp_listener::tcp_listener(io__::io_context &ios, end_point_wrapper &&endPoint)
    : acceptor_(ios, endPoint.point_), socketData_(std::make_unique<socket_data>(ios))
{
}

anet::listen::callback_function_wrapper::callback_function_wrapper(tcp_listener_ptr &&listener, callback_func_t &&handler)
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
    acceptor.async_accept(socketData->socket_, callback_function_wrapper(std::move(listener), std::move(handler)));
}