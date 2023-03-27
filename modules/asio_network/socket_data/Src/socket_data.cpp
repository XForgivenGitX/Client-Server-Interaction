#include "socket_data.hpp"

namespace anet
{
    socket_data::socket_data(const io__::any_io_executor &ios) : socket_(ios)
    {
    }

    void socket_data::shutdown() noexcept
    {
        if (!socket_.is_open())
            return;
        err_c error_c;
        socket_.shutdown(ip::tcp::socket::shutdown_both, error_c);
#ifdef NETWORK_ENABLE_HANDLER_TRACKING
        BOOST_LOG_TRIVIAL(info)
            << lg::build_log("shutdown socket",
                "sock: " + std::to_string(get_handle()), 
                "status: " + error_c.message());
#endif
        socket_.close();
    }
    ip_type socket_data::get_ip() const
    {
        return socket_.remote_endpoint().address().to_string();
    }
    std::size_t socket_data::get_handle()
    {
        return socket_.native_handle();
    }

    socket_data::~socket_data() { shutdown(); }

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