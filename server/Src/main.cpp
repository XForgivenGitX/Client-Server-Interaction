#include "ClientServer.hpp"

int main()
{
    setlocale(LC_ALL, "ru-RU");
    std::string data = "da";
    server::send("127.0.0.1", 9001, data);
    system("pause");
}