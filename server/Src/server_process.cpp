#include <server_handlers.hpp>

int main(int argc, char** argv) 
{ 
    if(argc != 3) 
    {
        std::cerr << "Incorrect arguments. Usage: <port> <clients_num>\n";
        return 1;
    }
    server::server_session::get_instanse().start(std::atoi(argv[1]), std::atoi(argv[2]));
    server::server_session::get_instanse().ios_.run();
}


