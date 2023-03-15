#include <server_module.hpp>
using namespace server;

int main(int argc, char** argv) 
{ 
    system("chcp 1251");
    if(argc < 2) 
    {
        std::cerr << "Incorrect arguments. Usage: <ports...> \n";
        return 1;
    }
    std::vector<io__::io_context> iocontexts(argc - 1);
    ptrs_to_servers servers;
    ptrs_to_workers workers;
    boost::thread_group tg;
    for(int i = 0; i < (argc - 1); ++i)
    {
        // workers.push_back(utility::safe_make_shared<worker_t>
        //             (io__::make_work_guard(iocontexts[i])));
        servers.push_back(utility::safe_make_shared<server_session>
                    (iocontexts[i], static_cast<unsigned short>(std::atoi(argv[i + 1]))));
    }
    std::for_each(servers.begin(), servers.end(), [](auto& server){server->start();});
    std::for_each(iocontexts.begin(), iocontexts.end(), [&tg](auto& ios)
    {
        tg.create_thread([&ios]{ios.run();});
    });
    tg.join_all();
}


