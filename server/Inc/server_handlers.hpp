#pragma once
#include <network_module.hpp>
#include <unordered_map>
// namespace server
// {
//     void send_handler(anet::socket_data_ptr &&socketData, const boost::system::error_code &error);
//     void receive_handler(anet::socket_data_ptr &&socketData, const boost::system::error_code &error);
//     void handler_accepted_connection(anet::tcp_listener_ptr &&listener, const boost::system::error_code &error);
// }

template <typename T>
struct singleton
{
    static T& get_instanse()
    {
        static T instance;
        return instance;
    }
protected:
    singleton() = default;
};

struct client
{
    using id_type = std::size_t;

public:
    anet::socket_data_ptr socketData;
    std::string name;
    id_type id;
};

struct handlers
{
    static void accepted_connection(anet::tcp_listener_ptr &&listener, const boost::system::error_code &error)
    {
        if(!error)
        {
            anet::send_receive::receive();
        }
        else
        {
            listener->socketData_->shutdown();
        }
    }
};


class server : public singleton<server>
{
    using connectedUser = client;

public:
    std::unordered_map<connectedUser::id_type, connectedUser> connectedUsers;
    connectedUser::id_type id_counter;
    static io__::io_context ios;

public:
    inline static void start(unsigned short port)
    {
        anet::end_point_wrapper endP(port, io__::ip::tcp::v4());
        
        while(true)
        {
            anet::listen::accepting_connection(utility::safe_make_unique<anet::tcp_listener>
                (ios, endP), std::function(handlers::accepted_connection));
        }
    }
};