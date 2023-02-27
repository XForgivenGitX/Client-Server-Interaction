#include "ClientServer.hpp"

server::ConnectionData::ConnectionData(io__::io_service &ios) : socket(ios)
{
}
void server::ConnectionData::shutdown()
{
    if (!socket.is_open())
        return;
    boost::system::error_code ignore;
    socket.shutdown(io__::ip::tcp::socket::shutdown_both, ignore);
    socket.close(ignore);
}
server::ConnectionData::~ConnectionData() { shutdown(); }

//TODO  
void server::async_write_data(ConnectionDataPtr &&dataPtr)
{
    auto& [socket, data] = *dataPtr;
    io__::io_service::strand _strand{detail::TaskProcessor::get_ios()};

    //io__::async_write(socket, io__::buffer(data), );
    //io__::async_write(socket, io__::buffer(data), task::TaskWrappedWithConnections<Func>(std::move(dataPtr), func));
}

void server::on_send(ConnectionDataPtr &&dataPtr, const boost::system::error_code &error)
{
}

void server::send()
{
    ConnectionDataPtr socket = detail::TaskProcessor::create_connection("94.29.6.202", 9001);
    socket->data = "PRIYOM";
    async_write_data(std::move(socket));
}