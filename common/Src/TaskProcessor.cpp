#include "ClientServer.hpp"
template <typename T>
T &detail::Singleton<T>::get_instance()
{
    static T instance;
    return instance;
}

io__::io_service &detail::TaskProcessor::get_ios()
{
    static io__::io_service ios;
    static io__::io_service::work work(ios);
    return ios;
}

template <typename T>
void detail::TaskProcessor::push_task(const T &task_unwrapped)
{
    get_ios().post(task::make_task_wrapped(task_unwrapped));
}

void detail::TaskProcessor::start()
{
    get_ios().run();
}

void detail::TaskProcessor::stop()
{
    get_ios().stop();
}

server::ConnectionDataPtr detail::TaskProcessor::create_connection(const std::string& address, const unsigned short port_num)
{
    server::ConnectionDataPtr connect = std::make_unique<server::ConnectionData>(get_ios());
    connect->socket.connect(io__::ip::tcp::endpoint(io__::ip::address_v4::from_string(address), port_num));
    return connect;
}
