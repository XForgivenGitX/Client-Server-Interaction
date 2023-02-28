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


<<<<<<< HEAD
template <typename Func>
void server::async_write_data(ConnectionDataPtr &&dataPtr, const Func& func)
{
    auto& [socket, data] = *dataPtr;
    io__::async_write(socket, io__::buffer(data), 
    [newPtr = std::move(dataPtr), &func](const boost::system::error_code& error, std::size_t bytes_transferred)
    {
        newPtr->data.resize(bytes_transferred);
        func(error, bytes_transferred);
    });
=======
void server::async_write_data(ConnectionDataPtr &&dataPtr)
{
    auto &[socket, data] = *dataPtr;
    io__::async_write(socket, io__::buffer(data), 
    [](const boost::system::error_code &error, std::size_t bytes_count) {});
>>>>>>> 7345dcb09b30e5c0a5026aa8c293c801325703b5
}

void server::send(const std::string& address, const unsigned short port, const std::string& data)
{
    ConnectionDataPtr socket;
    try
    {
        socket = detail::TaskProcessor::create_connection(address, port);
    }
    catch(std::exception& ex)
    {
        std::cerr << ex.what() << std::endl;
    }
    socket->data = data;
    async_write_data(std::move(socket), &on_send);
}

<<<<<<< HEAD
void server::on_send(const boost::system::error_code& error, std::size_t bytes_transferred)
{
    if(error)
    {
        std::cerr << error.message() << std::endl;
    }
    std::cout << "Transferred: " << bytes_transferred <<std::endl;
}

=======
void server::send(std::string& data)
{
    ConnectionDataPtr socket;
    try{ socket = detail::TaskProcessor::create_connection("127.0.0.1", 9001);}
    catch(std::exception& ex){std::cout << ex.what() <<'\n';}
    socket->data = data;
    async_write_data(std::move(socket));
}
>>>>>>> 7345dcb09b30e5c0a5026aa8c293c801325703b5
