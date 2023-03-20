#include <server_module.hpp>
using namespace common;


server::server_control_block::server_control_block()
    : pool_(boost::thread::hardware_concurrency()), lobby_(std::make_shared<simple_lobby>(&serverDataBase_))
{
}

void server::server_control_block::start_accepting_connections(unsigned short port)
{
    auto tcpListener = std::make_unique<anet::tcp_listener> 
        (pool_.get_executor(), anet::end_point_wrapper{port, io__::ip::tcp::v4()});
    auto& listenerSocket = tcpListener->socketData_;
    accept_connections(std::move(tcpListener)); 
    io__::signal_set set(signalIos_.get_executor(), SIGINT, SIGTERM, SIGABRT);
    set.async_wait(std::bind(&server_control_block::signal_handler, 
        this, listenerSocket, std::placeholders::_1, 
        std::placeholders::_2));
    signalIos_.run();
}

void server::server_control_block::accept_connections(anet::tcp_listener_ptr &&listener) noexcept
{
#ifdef SERVER_ENABLE_HANDLER_TRACKING
    std::cout << "@server=" << this << ": listen\n";
#endif
    anet::listen::accepting_connection(std::move(listener),
                                       {&server_control_block::accepted_connection_handler, this});
}

void server::server_control_block::accepted_connection_handler(anet::tcp_listener_ptr &&listener, const boost::system::error_code &error) noexcept
{
    if (error)
    {
        std::cerr << "@server=" << this << ": @" << error.message() << '\n';
        listener->socketData_->shutdown();
    }
    else
    {
        auto newSocket = std::make_shared<anet::socket_data>(pool_.get_executor());
        std::swap(newSocket, listener->socketData_);

#ifdef SERVER_ENABLE_HANDLER_TRACKING
        std::cout << "@server=" << this << ": @connect " << newSocket->socket_.remote_endpoint() << '\n';
#endif
        io__::post(pool_.get_executor(), std::bind(lobby::authorization, lobby_, newSocket));
        accept_connections(std::move(listener));
    }
}

void server::server_control_block::join()
{
    pool_.join();
}

void server::server_control_block::signal_handler(anet::socket_data_ptr listenerSocket, const boost::system::error_code error, int)
{
#ifdef SERVER_ENABLE_HANDLER_TRACKING
            std::cout << "@server=" << this << ": @called signal_handler\n";
#endif
    pool_.stop();
    listenerSocket->shutdown();
    lobby_->leave_all_users();
}