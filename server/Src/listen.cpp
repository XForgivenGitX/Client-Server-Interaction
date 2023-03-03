#include <ClientServer.hpp>

server::TcpListenerPtr server::create_listener(unsigned short port, sendReceive::callback_func_t &&func)
{
    try
    {
        server::TcpListenerPtr listener = std::make_unique<server::TcpListener>
        (detail::TaskProcessor::get_ios(), sendReceive::EndPointWrapper(port, io__::ip::tcp::v4()), std::move(func));
        return listener;
    }
    catch (const std::bad_alloc &ex)
    {
        std::cerr << ex.what() << std::endl;
        assert(false);
    }
}

void server::accepting_connection(server::TcpListenerPtr &&listener)
{
    if (!listener->acceptor_.is_open())
        return;
    auto &[acceptor, socketData, func] = *listener;
    try
    {
        socketData = std::make_unique<sendReceive::SocketData>(detail::TaskProcessor::get_ios());
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        assert(false);
    }
    acceptor.async_accept(socketData->socket, CallbackFunctionWrapper(std::move(listener)));
}

server::TcpListener::TcpListener(io__::io_service &ios, sendReceive::EndPointWrapper endPoint, sendReceive::callback_func_t &&func)
    : acceptor_(ios, endPoint.point), func_(std::move(func))
{
}