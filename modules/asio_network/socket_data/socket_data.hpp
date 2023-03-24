#pragma once
#include <string>
#include <iostream>

#include <boost/asio/io_service.hpp>
#include <boost/core/noncopyable.hpp>
#include <boost/system/error_code.hpp>
#include <boost/asio/ip/tcp.hpp>

namespace io__ = boost::asio;
typedef std::string transf_data;
typedef std::string addr_t;
typedef unsigned short port_t;
typedef boost::system::error_code error_code;

namespace anet
{
    struct socket_data : boost::noncopyable
    {
    public:    
        io__::ip::tcp::socket socket_;
        transf_data send_buffer_, receive_buffer_;
    
    public:   
        explicit socket_data(const io__::any_io_executor&);
        ~socket_data();
    
    public: 
        void shutdown() noexcept; 
    };
    typedef std::shared_ptr<socket_data> socket_data_ptr;

//_____

    struct end_point_wrapper
    {
    public:   
        io__::ip::tcp::endpoint point_;
        error_code error_;
    
    public:    
        end_point_wrapper(port_t port, const addr_t& address);
        end_point_wrapper(port_t port, const io__::ip::tcp& address);
        end_point_wrapper(){}
    };
    
    socket_data_ptr make_socket_data(const io__::any_io_executor& ios);

}