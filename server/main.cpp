#include <iostream>
#include <string>
#include <boost/system/error_code.hpp>
#include <TaskProcessor.hpp>
int main()
{
    using namespace boost::system;
    std::cout << "Enter command\n>";
    std::string command;
    while(std::cin >> command)
    {
        system(command.c_str());
        std::cout << ">>";
    }
    
}