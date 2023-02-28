
#include "ClientServer.hpp"

int main()
{
    setlocale( LC_ALL, "ru-RU" );
    std::string command("PRIOM");
    server::send(command);
    system("pause");
}