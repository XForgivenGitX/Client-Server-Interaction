#include "connections.hpp"
#include "main_lobby.hpp"
using namespace server;

int main(int argc, char** argv) 
{ 
    lg::init_logger();
    system("chcp 1251");
    // if(argc != 2) 
    // {
    //     std::cerr << "Incorrect arguments. Usage: <port> \n";
    //     return 1;
    // }
    auto& server = server::server_control_block::get_mutable_instance();
    server.start_accepting_connections(9001, std::make_shared<main_lobby>());
    //server.start_accepting_connections(std::atoi(argv[1]));
    server.join();
}


