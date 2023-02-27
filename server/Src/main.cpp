
#include "ClientServer.hpp"
#include <iostream>
int main()
{
  std::string command;
  while (1)
  {
      std::cout << ">>";
      //std::getline(std::cin, command);
      system(command.c_str());
  }
}