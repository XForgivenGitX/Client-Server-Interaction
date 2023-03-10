#pragma once
#include <network_module.hpp>
#include <unordered_map>
#include <unordered_set>
#include <condition_variable>

#define BOOST_THREAD_PROVIDES_THREAD_DESTRUCTOR_CALLS_TERMINATE_IF_JOINABLE
#include <boost/thread/thread.hpp>
#include <boost/asio/io_context_strand.hpp>

constexpr int FRAME_MAX_LENGHT = 128;
constexpr int FRAME_MIN_LENGHT = 5;

struct bad_server : std::exception
{
    virtual const char *what() const noexcept
    {
        return "server crashed on the initiative of the developer";
    }
    virtual ~bad_server() = default;
};

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

/////////////////////////////////////////////////////////////
namespace server
{
    class server_session : public singleton<server_session>
    {
        using connectedUser = anet::socket_data_ptr;
        using name_table = std::unordered_map<connectedUser, std::string>;
        using clients = std::unordered_set<connectedUser>;

    public:
        io__::io_context ios_;
        clients connectedUsers_;
        name_table names_;
        std::atomic<bool> server_stop_ = false;
        anet::end_point_wrapper endPoint_;
    
    public:
        inline void start(unsigned short port, std::size_t clientQuantity)
        {
            endPoint_ = anet::end_point_wrapper(port, io__::ip::tcp::v4());
            accept_connections(utility::safe_make_unique<anet::tcp_listener>(ios_, endPoint_));
            std::cout << "Server for " << clientQuantity << " clients started on " << port << " port.\n";
        }

        void accept_connections(anet::tcp_listener_ptr &&listener)
        {
            anet::listen::accepting_connection(std::move(listener), accepted_connection_handler);
        }
    
    private:
        static void accepted_connection_handler(anet::tcp_listener_ptr &&listener, const boost::system::error_code &error)
        {
            std::cout << "Client connected, system:" << error.message() << ".\n";
            auto newSocket = std::move(listener->socketData_);
            listener->socketData_ = utility::safe_make_unique<anet::socket_data>(get_instanse().ios_);
            if(error)
            {
                newSocket->shutdown();
            }
            else
            {
                anet::send_receive::receive(newSocket, authorization_handler, FRAME_MAX_LENGHT, FRAME_MIN_LENGHT);
            }
            anet::listen::accepting_connection(std::move(listener), accepted_connection_handler);
        }
        
        static void authorization_handler(anet::socket_data_ptr socketData, const boost::system::error_code &error)
        {
            auto& this_ = get_instanse();
            this_.connectedUsers_.insert(socketData);
            this_.names_[socketData] = socketData->data_;

        }
    };
}