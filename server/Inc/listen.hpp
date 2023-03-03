#pragma once
#include <ClientServer.hpp>

namespace server
{
    struct TcpListener;
    
    typedef io__::ip::tcp::acceptor acceptor_t;
    typedef std::unique_ptr<TcpListener> TcpListenerPtr;
    
    struct TcpListener
    {
    public:    
        acceptor_t acceptor_;
        sendReceive::SocketDataPtr socketData_;
        sendReceive::callback_func_t func_;
    
    public:
        explicit TcpListener(io__::io_service& ios, sendReceive::EndPointWrapper endPoint, sendReceive::callback_func_t&& func);
    };
    
    
    TcpListenerPtr create_listener(unsigned short port, sendReceive::callback_func_t&& func);
    void accepting_connection(TcpListenerPtr&& listener);
    void handler_connection(sendReceive::SocketDataPtr&&, const boost::system::error_code &);
    
    struct CallbackFunctionWrapper
    {
    public:
        explicit CallbackFunctionWrapper(TcpListenerPtr&& listener)
            : listener_(std::move(listener)) 
        {}
        void operator()(const boost::system::error_code& error)
        {
            auto socketData = std::move(listener_->socketData_);
            auto wrappedTask = task::make_task_wrapped(std::function(handler_connection));
            accepting_connection(std::move(listener_));
            wrappedTask(std::move(socketData), error);
        }
    public:
        TcpListenerPtr listener_;
    };
}


