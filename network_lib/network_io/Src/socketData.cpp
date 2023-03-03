#include <ClientServer.hpp>

sendReceive::SocketData::SocketData(io__::io_service &ios) : socket(ios)
{
}
void sendReceive::SocketData::shutdown()
{
    if (!socket.is_open()) return;
    boost::system::error_code ignore;
    socket.shutdown(io__::ip::tcp::socket::shutdown_both, ignore);
    socket.close(ignore);
}
sendReceive::SocketData::~SocketData() { shutdown(); }

sendReceive::EndPointWrapper::EndPointWrapper(unsigned short port, const std::string &address)
    : point(io__::ip::address::from_string(address, error), port)
{
}
sendReceive::EndPointWrapper::EndPointWrapper(unsigned short port, const io__::ip::tcp &address)
    : point(address, port)
{
}