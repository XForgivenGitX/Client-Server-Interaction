#include <client_module.hpp>

int main(int argc, char **argv)
{
    system("chcp 1251");
    io__::io_context ios;
    client::client_session client(ios, {9001, argv[1]}, std::string("Jim"));
    ios.run();
}
