#include <ClientServer.hpp>

using namespace client;
using namespace sendReceive;
using namespace std;

int main()
{
    setlocale(LC_ALL, "ru-RU");
    
    optional<SocketDataPtr> socketDataOpt;
    EndPointWrapper endPoint(9001, "127.0.0.1");
    
    socketDataOpt = Connection::connection_request(endPoint);
    if(socketDataOpt)
    {
        sendReceive::Action::receive(std::move(socketDataOpt.value()), 8, client::Handlers::receive_handler);
        detail::TaskProcessor::start();
    }
    else
    {
        return 1;
    }    
}
