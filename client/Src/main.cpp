#include <client_module.hpp>

int main(int argc, char **argv)
{
    system("chcp 1251");
    io__::io_context ios;
    client::client_session client(ios, {9001, argv[1]});
    std::thread th1([&ios]{ios.run();});
    std::thread th2([&ios]{ios.run();});
    th1.join();
    th2.join();
}
