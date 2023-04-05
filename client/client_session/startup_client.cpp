#include "client_session.hpp"

int main(int argc, char **argv)
{
    lg::init_logger();
    io__::io_context ios;
    client::client_session client(ios, {9001, "127.0.0.1"});
    auto lamd = [&ios]{ios.run();};
    std::thread th1(lamd), th2(lamd);
    th1.join();
    th2.join();
}
