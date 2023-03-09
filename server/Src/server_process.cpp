#include <server_handlers.hpp>

int main(int argc, char** argv) 
{ 
    if(argc != 3) 
    {
        std::cerr << "Encorret argument quantity\n";
        return 1;
    }
    server::server_session::get_instanse().start(std::atoi(argv[1]), std::atoi(argv[2]));
}


