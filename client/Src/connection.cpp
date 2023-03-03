#include <ClientServer.hpp>

std::optional<sendReceive::SocketDataPtr> client::Connection::connection_request(const sendReceive::EndPointWrapper endPoint)
{
    sendReceive::SocketDataPtr socketData;
    try
    {
        socketData = std::make_unique<sendReceive::SocketData>(detail::TaskProcessor::get_ios());
        socketData->socket.connect(endPoint.point);
    }
    catch (const std::bad_alloc &ex)
    {
        std::cerr << ex.what() << std::endl;
        assert(false);
    }
    catch (const boost::system::system_error &ex)
    {
        std::cerr << ex.what() << std::endl;
        return std::nullopt;
    }
    return socketData;
}

void client::Handlers::receive_handler(sendReceive::SocketDataPtr &&socketData, const boost::system::error_code &error)
{
    // auto& data = socketData->data;
    // data.erase(std::remove_if(data.begin(), data.end(), isspace), data.end());
    // if(data.c_str() == "disc") detail::TaskProcessor::stop();
    // else std::cout << data << std::endl;
    // sendReceive::Action::receive(std::move(socketData), 8, receive_handler);
    // return 0;
}