#include <iostream>
#include <string>
#include <boost/system/error_code.hpp>
#include <Inc/TaskProcessor.hpp>
#define __WIN32_WINNT 0x0601

int main()
{
    std::cout << "Enter command\n";
    std::string command;
    while(1)
    {
        std::cout << ">";
        std::getline(std::cin, command);
        system(command.c_str());
    }
}