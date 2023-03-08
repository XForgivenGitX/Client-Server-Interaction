#pragma once
#include <network_module.hpp>
#include <unordered_map>

#define BOOST_THREAD_PROVIDES_THREAD_DESTRUCTOR_CALLS_TERMINATE_IF_JOINABLE
#include <boost/thread/thread.hpp>

constexpr int FRAME_MAX_LENGHT = 128;
constexpr int FRAME_MIN_LENGHT = 5;

// namespace server
// {
//     void send_handler(anet::socket_data_ptr &&socketData, const boost::system::error_code &error);
//     void receive_handler(anet::socket_data_ptr &&socketData, const boost::system::error_code &error);
//     void handler_accepted_connection(anet::tcp_listener_ptr &&listener, const boost::system::error_code &error);
// }
template <typename T>
struct singleton
{
    static T &get_instanse()
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

class server : public singleton<server>
{
    using connectedUser = client;

public:
    static std::unordered_map<connectedUser::id_type, connectedUser> connectedUsers;
    static connectedUser::id_type id_counter;
    static io__::io_context ios;
    static boost::thread_group tg;
    static std::mutex mut;

public:
    struct handlers
    {
        static void send_handler(anet::socket_data_ptr &&socketData, const boost::system::error_code &error)
        {
            
            anet::send_receive::receive(std::move(socketData), std::function(receive2_handler), FRAME_MAX_LENGHT, FRAME_MIN_LENGHT);
        }
        
        static void receive_handler(anet::socket_data_ptr &&socketData, const boost::system::error_code &error)
        {
            mut.unlock();
            anet::send_receive::send(std::move(socketData), std::function(send_handler));
        }
        
        static void receive2_handler(anet::socket_data_ptr &&socketData, const boost::system::error_code &error)
        {
            anet::send_receive::send(std::move(socketData), std::function(send_handler));
        }
        
        static void accepted_connection(anet::tcp_listener_ptr &&listener, const boost::system::error_code &error)
        {
            
            if (!error)
            {
                anet::send_receive::receive(std::move(listener->socketData_), std::function(receive_handler), FRAME_MAX_LENGHT, FRAME_MIN_LENGHT);
            }
            else
            {
                listener->socketData_->shutdown();
            }
        }
    };

public:
    inline static void start(unsigned short port)
    {
        anet::end_point_wrapper endPoint(port, io__::ip::tcp::v4());
        std::cout << "Server on the " << port << " port has started. " << '\n';
        try
        {
            while (true)
            {
                mut.lock();
                anet::listen::accepting_connection
                    (utility::safe_make_unique<anet::tcp_listener>(ios, endPoint), std::function(handlers::accepted_connection));
                tg.create_thread([](){ ios.run(); });
            }
        }
        catch (...)
        {
            std::cout << "Server on the " << port << " port crashed." << '\n';
        }
    }
};