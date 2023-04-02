#include "acceptor.hpp"

namespace server
{
    acceptor::acceptor(anet::port_t port, const IConnections_manager_ptr& connectionManager)
        : pool_(boost::thread::hardware_concurrency()), connectionManager_(connectionManager)
    {
        start_accepting_connections(port);
    }

    void acceptor::start_accepting_connections(anet::port_t port)
    {
        anet::end_point_wrapper endp(port, io__::ip::tcp::v4());
        auto tcpListener = 
            std::make_unique<anet::tcp_listener>(pool_.get_executor(), endp);
        if(endp.error_c_) 
            throw std::runtime_error("the port or address of the endpoint is incorrect");
        start_signal_set(tcpListener->socketData_);

#ifdef ACCEPTOR_ENABLE_HANDLER_TRACKING
       BOOST_LOG_TRIVIAL(ACCEPTOR_LOG_SEVERITY) 
            << lg::build_src("start accepting conections")
#ifdef ACCEPTOR_DETAIL_LOG
            << lg::build_arg("port    : " + std::to_string(port))
#endif
            ;
#endif
        accept_connections(std::move(tcpListener));
    }

    void acceptor::start_signal_set(anet::socket_data_ptr &listenerSocket)
    {
        io__::post(pool_.get_executor(), [&listenerSocket, this]
        {
            io__::signal_set set(signalIos_.get_executor(), SIGINT, SIGTERM, SIGABRT);
            set.async_wait(std::bind(&acceptor::signal_handler,
                this, listenerSocket, std::placeholders::_1,
                std::placeholders::_2));
            signalIos_.run();
        });
    }

    void acceptor::accept_connections(anet::tcp_listener_ptr &&listener)
    {
        anet::listen::accepting_connection(std::move(listener),
                                           {&acceptor::accepted_connection_handler, this});
    }

    void acceptor::accepted_connection_handler(anet::tcp_listener_ptr &&listener, const anet::err_c &error_c) noexcept
    {
#ifdef ACCEPTOR_ENABLE_HANDLER_TRACKING
        BOOST_LOG_TRIVIAL(ACCEPTOR_LOG_SEVERITY) 
            << lg::build_src("accepted connection handler")
#ifdef ACCEPTOR_DETAIL_LOG
            << lg::build_arg("endp   : " + listener->socketData_->get_ip())
            << lg::build_arg("sock   : " + std::to_string(listener->socketData_->get_handle()))
            << lg::build_arg("status : " + error_c.message())
#endif 
            ;
#endif
        if (error_c)
        {
            anet::err_c ignore; 
            listener->socketData_->shutdown(ignore);
        }
        else
        {
            auto connectedSocket = std::make_shared<anet::socket_data>(pool_.get_executor());
            std::swap(connectedSocket, listener->socketData_);
            io__::post(pool_.get_executor(), 
                std::bind(&IConnections_manager::enter_user, connectionManager_, connectedSocket));
            accept_connections(std::move(listener));
        }
    }

    void acceptor::join()
    {
        pool_.join();
    }
    
    void acceptor::signal_handler(anet::socket_data_ptr listenerSocket, [[maybe_unused]] const anet::err_c &error_c, int)
    {
        anet::err_c shutdown_listener_status;
        
        pool_.stop();
        listenerSocket->shutdown(shutdown_listener_status);
        connectionManager_->leave_everyone();

#ifdef ACCEPTOR_ENABLE_HANDLER_TRACKING
        BOOST_LOG_TRIVIAL(warning) 
            << lg::build_src("signal handler")
#ifdef ACCEPTOR_DETAIL_LOG
            << lg::build_arg("signal  : " + error_c.message())
            << lg::build_arg("listener: " + shutdown_listener_status.message())
#endif
            ;
#endif
    }
}