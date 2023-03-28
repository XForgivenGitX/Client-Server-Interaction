#pragma once
#include "database.hpp"
#include "i_channels.hpp"
#include "listen_connect.hpp"
#include <boost/asio/thread_pool.hpp>
#include <boost/serialization/singleton.hpp>
#include <boost/asio/signal_set.hpp>

namespace server
{
    struct server_control_block : private db::server_database, public boost::serialization::singleton<server_control_block>
    {
        using db::server_database::insert_user;
        using db::server_database::erase_user;
        using db::server_database::insert_socket;
        using db::server_database::erase_socket;
        using db::server_database::find_and_insert_name;
        using db::server_database::check_user_data;
        using db::server_database::erase_name;
    
    private:
        io__::thread_pool pool_;
        io__::io_context signalIos_;
        ILobby_ptr lobby_;
        
    public:
        server_control_block();
        
    public:
        void join();
        void start_accepting_connections(anet::port_t port);

    private:
        void start_signal_set(anet::socket_data_ptr& listenerSocket);
        void signal_handler(anet::socket_data_ptr, const anet::err_c&, int);
        void accept_connections(anet::tcp_listener_ptr &&listener);
        void accepted_connection_handler(anet::tcp_listener_ptr &&listener, const anet::err_c &error) noexcept;
    };
}
