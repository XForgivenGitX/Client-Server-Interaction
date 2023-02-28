#include "ClientServer.hpp"

int main()
{
    setlocale(LC_ALL, "ru-RU");
    std::string data = "da";
    server::send("127.0.0.1", 9001, data);
    //server::send("127.0.0.1", 9001, data);
    //server::send("127.0.0.1", 9001, data);
    std::cout << "fff" << std::endl;
    system("pause");
    detail::TaskProcessor::start();
    detail::TaskProcessor::stop();
}