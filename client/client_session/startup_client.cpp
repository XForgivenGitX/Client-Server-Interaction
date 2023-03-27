#include "client_session.hpp"

int main(int argc, char **argv)
{
    system("chcp 1251");
    io__::io_context ios;
    client::client_session client(ios, {9001, argv[1]});
    auto lamd = [&ios]{ios.run();};
    std::thread th1(lamd), th2(lamd);
    th1.join();
    th2.join();
}
