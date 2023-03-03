#pragma once
#include <ClientServer.hpp>

namespace sendReceive
{
    struct SocketData;
    
    typedef std::unique_ptr<SocketData> SocketDataPtr;
    typedef std::function<void(SocketDataPtr&&, const boost::system::error_code &)> callback_func_t;
    
    struct SocketData : private boost::noncopyable
    {
        io__::ip::tcp::socket socket;
        std::string data;
        explicit SocketData(io__::io_service &ios);
        void shutdown();
        ~SocketData();
    };
    
    struct EndPointWrapper
    {
    public:   
        io__::ip::tcp::endpoint point;
        boost::system::error_code error;
    
    public:    
        EndPointWrapper(unsigned short port, const std::string& address);
        EndPointWrapper(unsigned short port, const io__::ip::tcp& address);
    };
    
    
}