// #include "ClientServer.hpp"
// #include <thread>
// #include <chrono>
// using namespace std::chrono_literals;
// int main()
// {
//   std::string command;
  
//   system("start calc");
//   std::this_thread::sleep_for(2000ms);
//   system("taskkill /im:calc");
  
//   while (std::getline(std::cin, command))
//   {
//       system(command.c_str());
//       std::cout << ">>";
//   }
// }

#include <boost/asio/io_service.hpp>
#include <boost/asio/steady_timer.hpp>
#include <chrono>
#include <thread>
#include <iostream>

using namespace boost::asio;

int main()
{
  io_service ioservice;

  steady_timer timer1{ioservice, std::chrono::seconds{3}};
  timer1.async_wait([](const boost::system::error_code &ec)
    { std::cout << "3 sec\n"; });

  steady_timer timer2{ioservice, std::chrono::seconds{3}};
  timer2.async_wait([](const boost::system::error_code &ec)
    { std::cout << "3 sec\n"; });

  std::thread thread1{[&ioservice](){ ioservice.run(); }};
  std::thread thread2{[&ioservice](){ ioservice.run(); }};
  std::cout << "rabota\n";
  
  thread1.join();
  thread2.join();
}
