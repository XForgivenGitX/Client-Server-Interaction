#pragma once
#include <network_module.hpp>
#include <unordered_map>
#include <condition_variable>

#define BOOST_THREAD_PROVIDES_THREAD_DESTRUCTOR_CALLS_TERMINATE_IF_JOINABLE
#include <boost/thread/thread.hpp>

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

struct client
{
    using id_type = std::size_t;

public:
    anet::socket_data_ptr socketData;
    std::string name;
    id_type id;
};
/////////////////////////////////////////////////////////////
namespace server
{
    struct handlers
    {
    };

    class server_session : public singleton<server_session>
    {
        using connectedUser = client;
        using clients = std::unordered_map<connectedUser::id_type, connectedUser>;
    public:
        ~server_session(){thread_gr.join_all();}
    
    
    public:
        io__::io_context ios;
        clients connectedUsers;
        boost::thread_group thread_gr;
        std::atomic<bool> server_stop = false;
        std::mutex connectedUserFlag;

    public:
        inline void start(unsigned short port, std::size_t clientQuantity)
        {
            anet::end_point_wrapper endPoint(port, io__::ip::tcp::v4());
            std::cout << "Server for " << clientQuantity << " clients started on " << port << " port.\n";
            try
            {
                while (!server_stop)
                {
                    connectedUserFlag.lock();
                    anet::listen::accepting_connection(utility::safe_make_unique<anet::tcp_listener>(ios, endPoint), accepted_connection_handler);
                    thread_gr.create_thread([this]{ ios.run(); });
                }
                throw bad_server{};
            }
            catch (std::exception &ex)
            {
                std::cerr << "Server on the " << port << " port crashed: " << ex.what() << '\n';
            }
            catch (...)
            {
                std::cerr << "Server on the " << port << " port crashed for an unknown reason.";
            }
        }

    private:
        static void accepted_connection_handler(anet::tcp_listener_ptr &&listener, const boost::system::error_code &error)
        {
            get_instanse().connectedUserFlag.unlock();
            std::cout << "Client connected, system:" << error.message() << ".\n";
            if(error){listener->socketData_->shutdown();}
            else
            {
                anet::send_receive::receive(std::move(listener->socketData_), authorization_handler, FRAME_MAX_LENGHT, FRAME_MIN_LENGHT);
            }
        }
        static void authorization_handler(anet::socket_data_ptr&& socketData, const boost::system::error_code &error)
        {
            static connectedUser::id_type id_count = 0;

        }
    };
}