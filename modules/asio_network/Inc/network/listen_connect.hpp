#pragma once
#include <network_module.hpp>

namespace anet
{
    struct connection
    {
        typedef std::function<void(socket_data_endpoint_ptr &&, const boost::system::error_code &)> callback_func_t;

    public:
        static void connection_request(socket_data_endpoint_ptr &&endPoint, callback_func_t &&);
        static void async_connection_request(io__::io_context &, socket_data_endpoint_ptr &&, callback_func_t &&);
    };

//_____
    
    struct tcp_listener
    {
    public:
        io__::ip::tcp::acceptor acceptor_;
        socket_data_ptr socketData_;

    public:
        explicit tcp_listener(io__::io_context &ios, const end_point_wrapper &endPoint);
        explicit tcp_listener(io__::io_context &ios, end_point_wrapper &&endPoint);
    };
    typedef std::unique_ptr<tcp_listener> tcp_listener_ptr;

//_____
    
    struct listen
    {
        typedef std::function<void(tcp_listener_ptr &&, const boost::system::error_code &)> callback_func_t;

    public:
        static void accepting_connection(tcp_listener_ptr&&, callback_func_t&& handler);
    
    private:
        struct callback_function_wrapper
        {
            tcp_listener_ptr listener_;
            callback_func_t functionUnwrapped_;

        public:
            explicit callback_function_wrapper(tcp_listener_ptr &&listener, callback_func_t &&handler);
            void operator()(const boost::system::error_code& error) noexcept;
        };
    };

}