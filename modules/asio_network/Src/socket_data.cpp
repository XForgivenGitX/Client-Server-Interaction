#include <network_module.hpp>

anet::socket_data::socket_data(io__::any_io_executor ios) : socket_(ios)
{
}

void anet::socket_data::shutdown() noexcept
{
    if (!socket_.is_open())
        return;
    try
    {
        socket_.shutdown(io__::ip::tcp::socket::shutdown_both);
        socket_.close();
    }
    catch (const boost::system::system_error &e)
    {
        std::cerr << e.what() << '\n';
#ifdef NETWORK_MODULE_RELEASE
        assert(false);
#endif
    }
}

anet::socket_data::~socket_data() { shutdown(); }

boost::system::error_code anet::socket_data_endpoint::connect() const noexcept
{
    if (socketData_->socket_.is_open())
        return {};
    boost::system::error_code error;
    socketData_->socket_.connect(endPoint_.point_, error);
    return error;
}

anet::socket_data_endpoint::socket_data_endpoint(socket_data_ptr socketData, end_point_wrapper &endPoint)
    : socketData_(socketData), endPoint_(std::move(endPoint))
{
}

anet::socket_data_endpoint_ptr anet::make_socket_data(io__::any_io_executor ios, end_point_wrapper &endPoint)
{
    return std::make_unique<socket_data_endpoint>
        (std::make_shared<socket_data>(ios), endPoint);
}

anet::socket_data_endpoint_ptr anet::make_socket_data(socket_data_ptr& socketData, end_point_wrapper& endPoint)
{
    return std::make_unique<socket_data_endpoint>(socketData, endPoint);
}

anet::socket_data_ptr anet::make_socket_data(io__::any_io_executor ios)
{
    return std::make_shared<socket_data>(ios);
}

anet::end_point_wrapper::end_point_wrapper(unsigned short port, const std::string &address)
    : point_(io__::ip::make_address(address, error_), port)
{
}

anet::end_point_wrapper::end_point_wrapper(unsigned short port, const io__::ip::tcp &address)
    : point_(address, port)
{
}
