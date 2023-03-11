#pragma once
#include <deque>
#include <unordered_map>

#define BOOST_THREAD_PROVIDES_THREAD_DESTRUCTOR_CALLS_TERMINATE_IF_JOINABLE

#include <network_module.hpp>
#include <boost/thread/thread.hpp>
#include <boost/asio/io_context_strand.hpp>
#include <boost/asio/executor_work_guard.hpp>
#include <boost/enable_shared_from_this.hpp>

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
    class client_session
    {
    public:   
        std::string name;
        server_session::ptr myServer;

    
    };
    
    
    class server_session :  public boost::noncopyable,
                            public std::enable_shared_from_this<server_session>
    {
    public: 
        using connectedUser = anet::socket_data_ptr;
        using name_table    = std::unordered_map<connectedUser, std::string>;
        using clients       = std::unordered_map<connectedUser, client_session>;
        using all_messages  = std::deque<std::string>;
        using ptr           = std::shared_ptr<server_session>;
    
    public:
        io__::io_context& ios_;
        const anet::end_point_wrapper endPoint_;
        clients connectedUsers_;
        name_table names_;
        
    
    public:
        server_session(io__::io_context& ios, unsigned short port) 
            : ios_(ios), endPoint_(port, io__::ip::tcp::v4()){}
    
    public:
        inline void start()
        {
            accept_connections(utility::safe_make_unique<anet::tcp_listener>(ios_, endPoint_));
            std::cout << "Server was started on " << endPoint_.point_.port() << " port.\n";
        }
    
    private:
        void accept_connections(anet::tcp_listener_ptr &&listener)
        {
            anet::listen::accepting_connection(std::move(listener), [this](anet::tcp_listener_ptr &&listener, const boost::system::error_code &error)
            {this->accepted_connection_handler(std::move(listener), error);});
        }
    
        void accepted_connection_handler(anet::tcp_listener_ptr &&listener, const boost::system::error_code &error)
        {
            auto newSocket = std::move(listener->socketData_);
            listener->socketData_ = utility::safe_make_unique<anet::socket_data>(ios_);
            if(error)
            {
                std::cerr << "Connection error, system:"<< error.message() << ".\n";
                newSocket->shutdown();
            }
            else
            {
                std::cout << "Successful connection.\nSystem:"
                << error.message() 
                << "\nIPv4: " 
                << newSocket->socket_.remote_endpoint().address().to_string() 
                << '\n';
                
            }
            accept_connections(std::move(listener));
        }
    };
}