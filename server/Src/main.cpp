<<<<<<< HEAD
=======

>>>>>>> 7345dcb09b30e5c0a5026aa8c293c801325703b5
#include "ClientServer.hpp"

int main()
{
<<<<<<< HEAD
    setlocale(LC_ALL, "ru-RU");
    std::string data = "da";
    server::send("127.0.0.1", 9001, data);
    //server::send("127.0.0.1", 9001, data);
    //server::send("127.0.0.1", 9001, data);
    std::cout << "fff" << std::endl;
    system("pause");
    detail::TaskProcessor::start();
    detail::TaskProcessor::stop();
=======
    setlocale( LC_ALL, "ru-RU" );
    std::string command("PRIOM");
    server::send(command);
    system("pause");
>>>>>>> 7345dcb09b30e5c0a5026aa8c293c801325703b5
}