#pragma once
#include <network_module.hpp>

namespace anet
{
    struct socket_data : boost::noncopyable
    {
    public:    
        io__::ip::tcp::socket socket_;
        data_t send_buffer_, receive_buffer_;
    
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
        boost::system::error_code error_;
    
    public:    
        end_point_wrapper(unsigned short port, const std::string& address);
        end_point_wrapper(unsigned short port, const io__::ip::tcp& address);
        end_point_wrapper(){}
    };
    
    socket_data_ptr make_socket_data(const io__::any_io_executor& ios);

}