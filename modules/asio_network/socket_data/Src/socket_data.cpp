#include "socket_data.hpp"

namespace anet
{
    socket_data::socket_data(const io__::any_io_executor &ios) : socket_(ios)
    {
    }

    void socket_data::shutdown(err_c& error_c) noexcept
    {
        socket_.shutdown(ip::tcp::socket::shutdown_both, error_c);
        socket_.close(error_c);
    }
    
    ip_type socket_data::get_ip() const
    {
        return socket_.remote_endpoint().address().to_string();
    }
    
    std::size_t socket_data::get_handle()
    {
        return socket_.native_handle();
    }

    io__::any_io_executor socket_data::get_executor()
    {
        return socket_.get_executor();
    }

    socket_data::~socket_data() 
    { 
        err_c ignore;
        shutdown(ignore);
    }

    socket_data_ptr make_socket_data(const io__::any_io_executor &ios)
    {
        return std::make_shared<socket_data>(ios);
    }

    end_point_wrapper::end_point_wrapper(port_t port, const ip_type &address)
        : point_(ip::make_address(address, error_c_), port)
    {
    }

    end_point_wrapper::end_point_wrapper(port_t port, const ip::tcp &address)
        : point_(address, port)
    {
    }
}