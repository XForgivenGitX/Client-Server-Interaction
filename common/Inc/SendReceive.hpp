#pragma once
#include <ClientServer.hpp>

namespace SendReceive
{
    struct NetConfig
    {
        io__::ip::tcp::endpoint point;
        boost::system::error_code error;
        NetConfig(const std::string& address,  const unsigned short port)
        : point(io__::ip::address::from_string(address, error), port)
        {}
    };
    
    struct Bidirectional
    {
        using handler_func_t = std::function<void(server::SocketDataPtr&&, std::size_t)>;
    
    public:
        static void send(server::SocketDataPtr&& socketData, const handler_func_t&& func);
        static void receive(server::SocketDataPtr&& socketData, const handler_func_t&& func);
    
    private:   
        Bidirectional() = delete;
    };

}