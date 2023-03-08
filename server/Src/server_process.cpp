// #include <server_handlers.hpp>

// int main()
// {
//     io__::io_context ios;
//     anet::end_point_wrapper end(9001, io__::ip::tcp::v4());
//     anet::listen::accepting_connection(utility::safe_make_unique<anet::tcp_listener>
//         (ios, end), std::function(server::handler_accepted_connection));
//     ios.run();
// }


#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <windows.h>
typedef boost::asio::ip::tcp tcp;
 
int main ()
{
    //SetConsoleOutputCP(1251);
    boost::system::error_code ec;
    std::string name=boost::asio::ip::host_name(ec);
    if(ec)
    {
        std::cerr<<"host_name() failed: \n"<<ec.message();
        return __LINE__;
    }
    else
        std::cout << "host name is " << name << std::endl;
    boost::asio::io_service io;
    tcp::resolver r(io);
    tcp::resolver::iterator it = r.resolve(tcp::resolver::query(name, ""), ec), itEnd;
    if(ec)
    {
        std::cerr<<"resolve() failed: \n"<<ec.message();
        return __LINE__;
    }
    for(; it != itEnd; ++it)
    {
        std::cout << it->endpoint().address().to_string() << std::endl;
    }
    return 0;
}