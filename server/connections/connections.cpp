#include "connections.hpp"
#include "chat_lobby.hpp"

namespace server
{
    server_control_block::server_control_block()
        : pool_(boost::thread::hardware_concurrency()), lobby_(std::make_shared<server::main_lobby>())
    {
    }

    void server_control_block::start_accepting_connections(anet::port_t port)
    {
        auto tcpListener = std::make_unique<anet::tcp_listener>(pool_.get_executor(), anet::end_point_wrapper{port, io__::ip::tcp::v4()});
        auto &listenerSocket = tcpListener->socketData_;
        accept_connections(std::move(tcpListener));

#ifdef SERVER_ENABLE_HANDLER_TRACKING
       BOOST_LOG_TRIVIAL(info) 
            << lg::build_log(lg::address_cat("server=", this), 
                "start accepting conections", 
                "port: " + std::to_string(port));
#endif
        start_signal_set(listenerSocket);
    }

    void server_control_block::start_signal_set(anet::socket_data_ptr &listenerSocket)
    {
        io__::post(pool_.get_executor(), [&listenerSocket, this]
        {
            io__::signal_set set(signalIos_.get_executor(), SIGINT, SIGTERM, SIGABRT);
            set.async_wait(std::bind(&server_control_block::signal_handler,
                this, listenerSocket, std::placeholders::_1,
                std::placeholders::_2));
            signalIos_.run();
        });
    }

    void server_control_block::accept_connections(anet::tcp_listener_ptr &&listener)
    {
        anet::listen::accepting_connection(std::move(listener),
                                           {&server_control_block::accepted_connection_handler, this});
    }

    void server_control_block::accepted_connection_handler(anet::tcp_listener_ptr &&listener, const anet::err_c &error_c) noexcept
    {
#ifdef SERVER_ENABLE_HANDLER_TRACKING
        BOOST_LOG_TRIVIAL(info) 
            << lg::build_log(lg::address_cat("server=", this), 
                "called accepted connection handler", 
                "endp: " + listener->socketData_->get_ip(), 
                "sock: " + std::to_string(listener->socketData_->get_handle()),
                "status: " + error_c.message());
#endif
        if (error_c)
        {
            listener->socketData_->shutdown();
        }
        else
        {
            auto newSocket = std::make_shared<anet::socket_data>(pool_.get_executor());
            std::swap(newSocket, listener->socketData_);
            io__::post(pool_.get_executor(), std::bind(ILobby::enter_user, lobby_, newSocket));
            accept_connections(std::move(listener));
        }
    }

    void server_control_block::join()
    {
        pool_.join();
    }
    
    void server_control_block::signal_handler(anet::socket_data_ptr listenerSocket, [[maybe_unused]] const anet::err_c &error_c, int)
    {
#ifdef SERVER_ENABLE_HANDLER_TRACKING
        BOOST_LOG_TRIVIAL(warning) 
            << lg::build_log(lg::address_cat("server=", this), 
                "called signal handler", 
                "status: " + error_c.message());
#endif
        pool_.stop();
        listenerSocket->shutdown();
        // lobby_->leave_all_users();
    }
}