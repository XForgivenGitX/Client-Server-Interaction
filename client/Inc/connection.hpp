#pragma once
#include <ClientServer.hpp>

//TODO
//1.client command: connect, exit, send 
//2.RAII class for handlers

namespace client
{
    struct Connection
    {
    public:
        static std::optional<sendReceive::SocketDataPtr> connection_request(const sendReceive::EndPointWrapper endPoint);
        
    private:
        Connection() = delete;
    };

    struct Handlers
    {
    public:
        static void send_handler(sendReceive::SocketDataPtr&& socketData, const boost::system::error_code& error);
        static void receive_handler(sendReceive::SocketDataPtr&& socketData, const boost::system::error_code& error);   
    private:
        Handlers() = delete;
    };
}
