#include <server_module.hpp>
using namespace server;

int main(int argc, char** argv) 
{ 
    if(argc != 2) 
    {
        std::cerr << "Incorrect arguments. Usage: <port> \n";
        return 1;
    }
    server_control_block server(std::atoi(argv[1]));
    server.start_accepting_connections();
    server.pool_.join();
}


