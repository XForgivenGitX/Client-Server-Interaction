#pragma once

#include <boost/asio/thread_pool.hpp>
#include <boost/asio/signal_set.hpp>

#include "listen_connect.hpp"
#include "interface.hpp"
#include "logger.hpp"

namespace server
{
    struct acceptor final
    {
    private:
        io__::thread_pool pool_;
        io__::io_context signalIos_;
        IConnections_manager_ptr connectionManager_;
        
    public:
        acceptor(anet::port_t port, const IConnections_manager_ptr& connectionManager);
        void join();
        
    private:
        void start_accepting_connections(anet::port_t port);
        void start_signal_set(anet::socket_data_ptr& listenerSocket);
        void signal_handler(anet::socket_data_ptr, const anet::err_c&, int);
        void accept_connections(anet::tcp_listener_ptr &&listener);
        void accepted_connection_handler(anet::tcp_listener_ptr &&listener, const anet::err_c &error) noexcept;
    };
}
