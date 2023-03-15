#include <client_module.hpp>

using resolver = io__::ip::tcp::resolver;

int main(int argc, char **argv)
{
    system("chcp 1251");
    if (argc != 4)
    {
        std::cerr << "Incorrect arguments. Usage: <nickname> <host> <port>\n";
        return 1;
    }

    io__::io_context ios;
    //resolver res(ios);
    anet::end_point_wrapper endPoint(std::atoi(argv[3]), argv[2]);
    //boost::system::error_code error;
    //auto endpoints = res.resolve(endPoint.point_, error);
    if(endPoint.error_)
    {
        std::cerr << "Invalid host or port, try again:" << endPoint.error_.message() << ".\n";
        return 1;
    }
    client::client_session user(ios, endPoint, argv[1]);
    user.start();
    boost::thread th([&ios]{ios.run();});
    std::string buffer;
    while(1)
    {
        std::getline(std::cin, buffer);
        user.send_message(buffer);
    }
    th.join();
}
