#include <client_handlers.hpp>

int main()
{
    io__::io_context ios;
    anet::end_point_wrapper end(9001, "192.168.56.1");
    anet::connection::connection_request
        (anet::make_socket_data(ios, std::move(end)), std::function(client::connection_handler));
    ios.run();
}
