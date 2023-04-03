#pragma once

#include <string>
#include <boost/asio/io_service.hpp>
#include <boost/core/noncopyable.hpp>
#include <boost/system/error_code.hpp>
#include <boost/asio/write.hpp>
#include <boost/asio/read.hpp>
#include <boost/asio/ip/tcp.hpp>
#include "utility.hpp"

namespace io__      = boost::asio;
namespace anet
{
    namespace ip      = io__::ip;
    using err_c = boost::system::error_code;
    
    typedef std::string transf_data;
    typedef std::string ip_type;
    typedef unsigned short port_t;
    
    struct socket_data : boost::noncopyable
    {
    public:    
        ip::tcp::socket socket_;
        transf_data send_buffer_, receive_buffer_;
    
    public:   
        explicit socket_data(const io__::any_io_executor&);
        ~socket_data();
    
    public: 
        void shutdown(err_c& error_c) noexcept; 
        ip_type get_ip() const;
        std::size_t get_handle();
        io__::any_io_executor get_executor();
    };
    typedef std::shared_ptr<socket_data> socket_data_ptr;

//_____

    struct end_point_wrapper
    {
    public:   
        ip::tcp::endpoint point_;
        err_c error_c_;
    
    public:    
        end_point_wrapper(port_t port, const ip_type& address);
        end_point_wrapper(port_t port, const io__::ip::tcp& address);
        end_point_wrapper(){}
    };
    
    socket_data_ptr make_socket_data(const io__::any_io_executor& ios);

}