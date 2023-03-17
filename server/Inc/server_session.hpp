#pragma once
#include <server_module.hpp>
#include <unordered_set>
#include <boost/serialization/singleton.hpp>
#include <vector>

using namespace std::literals::string_literals;
namespace server
{
    enum class command : unsigned
    {

    };
    
    //<5>%<Oleg><1234567890>
    std::string assemble_frame(command cmd, std::vector<std::string> args)
    {
        std::string frame("[<"s + std::to_string(static_cast<unsigned>(cmd)) + ">%"s);
        for(auto& arg : args)
            frame += "<"s + arg + ">"s;
        return frame + "]"s;
    }
    
    
    struct user
    {
    public:
        std::string name_, pass_;
        std::atomic<bool> status;

    public:
        user(std::string name, std::string pass) : name_(name), pass_(pass) {}
        // bool is_name(std::string &rhs)
        // {
        //     return name_ == rhs;
        // }
        // bool operator==(access_user &rhs)
        // {
        //     return (rhs.name_ == name_) == (rhs.pass_ == pass_);
        // }
    };

    class user_database : public boost::noncopyable
    {
        using ip_type = std::string;
        using name_t = std::string;
        
        using users_config_tree = std::map<ip_type, user>;
        using users_names_tree = std::set<std::string>;
        using room_config_table = std::unordered_map<std::string, chat_room>;

    private:
        users_config_tree usersConfig;
        users_names_tree usersNames;
        room_config_table activeRooms;

        mutable std::shared_mutex mut;

    public: // find, set and get methods
        //name_t is_name_occupied()
        bool is_register_user(ip_type ip)
        {
            std::shared_lock lock(mut);
            return usersConfig.find(ip) != usersConfig.end();
        }
    };
    class chatLobby
    {
        user_database dataBase;
    public:
        void authorization(anet::socket_data_ptr socketData)
        {
            if(dataBase.is_register_user(socketData->socket_.remote_endpoint().address().to_string()))
            {
                //log in
            }
            else
            {
                //register
            }
        }
    };

    class server_control_block : 
                        public boost::noncopyable, 
                        public boost::serialization::singleton<server_control_block>
    {
        using connectedSockets = std::unordered_set<anet::socket_data_ptr>;

    public:
        io__::thread_pool pool_;
        const anet::end_point_wrapper endPoint_;
        connectedSockets allUsers_;
        chatLobby lobby_;
    
    public:
        server_control_block(unsigned short port)
            : pool_(boost::thread::hardware_concurrency()), endPoint_(port, io__::ip::tcp::v4()) {}

        void start_accepting_connections() // timeout
        {
            accept_connections(std::make_unique<anet::tcp_listener>(pool_, endPoint_)); // ex new
        }

    private:
        void accept_connections(anet::tcp_listener_ptr &&listener) noexcept
        {
#ifdef SERVER_ENABLE_HANDLER_TRACKING
            std::cout << " @server=" << this << ": @listens " << endPoint_.point_.port() << '\n';
#endif
            anet::listen::accepting_connection(std::move(listener),
                                               {&server_control_block::accepted_connection_handler, this});
        }

        void accepted_connection_handler(anet::tcp_listener_ptr &&listener, const boost::system::error_code &error) noexcept
        {
            if (error)
            {
                std::cerr << " @server=" << this << ": @" << error.message() << '\n';
                listener->socketData_->shutdown();
            }
            else
            {
                auto newSocket = std::make_shared<anet::socket_data>(pool_);
                std::swap(newSocket, listener->socketData_);

#ifdef SERVER_ENABLE_HANDLER_TRACKING
                std::cout << " @server=" << this << ": @connect " << newSocket->socket_.remote_endpoint() << '\n';
#endif
                allUsers_.insert(newSocket);
                io__::post(pool_, std::bind(chatLobby::authorization, lobby_, newSocket));
                accept_connections(std::move(listener));
            }
        }
    };

    class server_session : public boost::noncopyable
    {
#if i
    private:
        io__::io_context &ios_;
        const anet::end_point_wrapper endPoint_;
        chat_room room_;

    public:
        server_session(io__::io_context &ios, unsigned short port);
        void start();

    private:
        void accepted_connection_handler(anet::tcp_listener_ptr &&listener, const boost::system::error_code &error) noexcept;
        void accept_connections(anet::tcp_listener_ptr &&listener) noexcept;
        void authorization_handler(anet::socket_data_ptr socketData, const boost::system::error_code &error) noexcept;
#endif
    };
}