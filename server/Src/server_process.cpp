#include <server_handlers.hpp>

int main(int argc, char** argv) 
{ 
    if(argc < 2) 
    {
        std::cerr << "Incorrect arguments. Usage: <ports...> \n";
        return 1;
    }
    io__::io_context ios;
    io__::executor_work_guard worker = io__::make_work_guard(ios);
    server::server_session server(ios, std::atoi(argv[1]));
    server.start();
    ios.run();
}


