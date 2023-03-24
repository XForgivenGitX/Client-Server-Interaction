#pragma once

#include <utility.hpp>
#include <socket_data.hpp>

namespace anet
{
    struct connection
    {
        typedef utility::task_wrapped<void(socket_data_ptr&, 
                                    const error_code &)> callback_func_t;

    public:
        static void connection_request(socket_data_ptr socketData, 
                                    const end_point_wrapper &endPoint, 
                                        callback_func_t &&handler);
        
        static void async_connection_request(const io__::any_io_executor& ios, 
                                            socket_data_ptr socketData, 
                                            const end_point_wrapper &endPoint, 
                                                    callback_func_t &&handler);
    };

    
    struct tcp_listener : boost::noncopyable
    {
    public:
        io__::ip::tcp::acceptor acceptor_;
        socket_data_ptr socketData_;

    public:
        explicit tcp_listener(const io__::any_io_executor& ios, 
                                    const end_point_wrapper &endPoint);
    };
    typedef std::unique_ptr<tcp_listener> tcp_listener_ptr;


    struct listen
    {
        typedef utility::task_wrapped<void(tcp_listener_ptr &&, 
                                    const error_code &)> callback_func_t;

    public:
        static void accepting_connection(tcp_listener_ptr &&, 
                                        callback_func_t &&handler);
    private:
        struct callback_function_wrapper;
    };
    
    struct listen::callback_function_wrapper
    {
    public:
        tcp_listener_ptr listener_;
        callback_func_t functionUnwrapped_;

    public:
        explicit callback_function_wrapper(tcp_listener_ptr &&listener, 
                                            callback_func_t &&handler);
        
        void operator()(const error_code &error) noexcept;
    };
}