#include <server_handlers.hpp>

std::unordered_map<server::connectedUser::id_type, server::connectedUser> server::connectedUsers;
server::connectedUser::id_type server::id_counter;
io__::io_context server::ios;
boost::thread_group server::tg;
std::mutex server::mut;