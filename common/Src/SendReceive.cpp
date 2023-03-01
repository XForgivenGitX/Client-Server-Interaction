#include "ClientServer.hpp"

// TODO
// 1.rename server namespace
// 3.io__::buffer accepts a non-const argument
// 4.add debug block instead of assert
// 5.add wrapper to call a user function
// 9.add compile-time condition on the correctness of user function arguments

void SendReceive::Bidirectional::send(server::SocketDataPtr &&socketData, const handler_func_t &&func)
{
    auto &[socket, data] = *socketData;
    io__::async_write(socket, io__::buffer(data), [socketData_ = std::move(socketData), func_ = std::move(func)]
        (const boost::system::error_code &error, const std::size_t bytes_transferred) mutable
        {
            
            func_.operator()(std::move(socketData_), bytes_transferred);
        });
}
// server::SocketDataPtr SendReceive::Bidirectional::create_connection(const NetConfig endPoint)
// {
//     server::SocketDataPtr socketData = std::make_unique<server::ConnectionData>(detail::TaskProcessor::get_ios());
//     socketData->socket.connect(endPoint.point);
//     return socketData;
// }
// void SendReceive::Bidirectional::send(const NetConfig& endPoint, const std::string& data, const handler_func_t func)
// {
//     server::SocketDataPtr socketData;
//     try
//     {
//         socketData = create_connection(endPoint);
//     }
//     catch (boost::system::system_error &ex)
//     {
//         std::cerr << ex.what() << std::endl;
//         return;
//     }
//     socketData->data = data;
//     async_write_data(std::move(socketData), std::move(func));
// }

void SendReceive::Bidirectional::receive(server::SocketDataPtr &&socketData, const handler_func_t &&func)
{
    auto &[socket, data] = *socketData;

}
