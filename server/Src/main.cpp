#include "ClientServer.hpp"

int main()
{
    setlocale(LC_ALL, "ru-RU");
    std::string data = "MISHANYA";
    //SendReceive::Bidirectional::send({"127.0.0.1", 9001}, data, &server::handler_server_send);
    detail::TaskProcessor::start();
}