#include <server_handlers.hpp>

int main()
{
    io__::io_context ios;
    anet::end_point_wrapper end(9001, io__::ip::tcp::v4());
    anet::listen::accepting_connection(utility::safe_make_unique<anet::tcp_listener>
        (ios, end), std::function(server::handler_accepted_connection));
    ios.run();
}
