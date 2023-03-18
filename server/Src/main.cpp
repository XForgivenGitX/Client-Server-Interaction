#include <server_module.hpp>
using namespace server;

int main(int argc, char** argv) 
{ 
    system("chcp 1251");
    // if(argc != 2) 
    // {
    //     std::cerr << "Incorrect arguments. Usage: <port> \n";
    //     return 1;
    // }
    auto& server = server_control_block::get_mutable_instance();
    server.start_accepting_connections(9001);
    server.pool_.join();
}


