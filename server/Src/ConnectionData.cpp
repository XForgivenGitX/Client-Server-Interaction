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


void server::async_write_data(ConnectionDataPtr &&dataPtr)
{
    auto &[socket, data] = *dataPtr;
    io__::async_write(socket, io__::buffer(data), 
    [](const boost::system::error_code &error, std::size_t bytes_count) {});
}

void server::on_send(ConnectionDataPtr &&dataPtr, const boost::system::error_code &error)
{
}

void server::send(std::string& data)
{
    ConnectionDataPtr socket;
    try{ socket = detail::TaskProcessor::create_connection("127.0.0.1", 9001);}
    catch(std::exception& ex){std::cout << ex.what() <<'\n';}
    socket->data = data;
    async_write_data(std::move(socket));
}