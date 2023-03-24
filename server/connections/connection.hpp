#pragma once
#include <boost/asio/thread_pool.hpp>
#include <boost/noncopyable.hpp>
#include <boost/serialization/singleton.hpp>

namespace server
{
    struct server_control_block : public boost::noncopyable, public boost::serialization::singleton<server_control_block>
    {
    private:
        io__::thread_pool pool_;
        io__::io_context signalIos_;
        lobby_ptr lobby_;
        
    public:
        db::server_database serverDataBase_{};
        
    public:
        server_control_block();
        
    public:
        void join();
        void start_accepting_connections(unsigned short port);
        
    private:
        void signal_handler(anet::socket_data_ptr, const boost::system::error_code, int);
        void accept_connections(anet::tcp_listener_ptr &&listener) noexcept;
        void accepted_connection_handler(anet::tcp_listener_ptr &&listener, 
                                const boost::system::error_code &error) noexcept;
    };
}