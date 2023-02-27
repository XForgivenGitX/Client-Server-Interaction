#include "ClientServer.hpp"

server::ConnectionData::ConnectionData(io__::io_service &ios) : socket(ios)
{}
void server::ConnectionData::shutdown()
{
    if (!socket.is_open())
        return;
    boost::system::error_code ignore;
    socket.shutdown(io__::ip::tcp::socket::shutdown_both, ignore);
    socket.close(ignore);
}
server::ConnectionData::~ConnectionData() { shutdown(); }