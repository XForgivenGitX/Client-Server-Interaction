#pragma once
#include <server_module.hpp>
#include <unordered_set>
#include <boost/serialization/singleton.hpp>
#include <protocol.hpp>
#include <map>

using namespace common;
namespace server
{
    
    struct registered_user
    {
    public:
        std::string name_, pass_;

    public:
        registered_user(const std::string& name, const std::string& pass) 
            : name_(name), pass_(pass) {}
        registered_user() = default;
    };

    class user_database : public boost::noncopyable
    {
    public:
        using ip_type = std::string;
        using name_t = std::string;
        
        typedef std::map<ip_type, registered_user> users_config_tree;
        typedef std::set<std::string>   users_names_tree;
        //typedef std::unordered_map<std::string, chat_room> room_config_table;

    private:
        users_config_tree usersConfig;
        users_names_tree usersNames;
        //room_config_table activeRooms;

        mutable std::shared_mutex mut;

    public:
        bool is_register_user(const ip_type& ip) const
        {
            std::shared_lock lock(mut);
            return usersConfig.contains(ip);
        }

        bool is_contains_name(const name_t& name) const
        {
            std::shared_lock lock(mut);
            return usersNames.contains(name);
        }
        
        bool is_valid_user_data(const ip_type& ip, const name_t& name, const name_t& pass)//
        {
            std::shared_lock lock(mut);
            auto& [valid_name, valid_pass] = usersConfig[ip];
            return (valid_name == name) && (valid_pass == pass);
        }
    
    public:
        void add_user(const ip_type& ip, registered_user&& data)
        {
            std::lock_guard lock(mut);
            usersNames.insert(data.name_);
            usersConfig.insert({ip, std::move(data)});
        }

        
    };
    class chatLobby
    {
        user_database dataBase;
    public:
        void authorization(anet::socket_data_ptr socketData);

        void auth_request_handler(anet::socket_data_ptr socketData, const boost::system::error_code &error) noexcept;
    
        void user_responce_handler(anet::socket_data_ptr socketData, const boost::system::error_code &error) noexcept;

    };

    class server_control_block : 
                        public boost::noncopyable, 
                        public boost::serialization::singleton<server_control_block>
    {
        typedef std::unordered_map<anet::socket_data_ptr, user_database::ip_type> connected_ip_table;
    public:
        io__::thread_pool pool_;
        connected_ip_table connectedIP;
        chatLobby lobby_;
    
    protected:
        server_control_block()
            : pool_(boost::thread::hardware_concurrency()) {}

    public:
        void start_accepting_connections(unsigned short port) // timeout
        {
            accept_connections(std::make_unique<anet::tcp_listener>(pool_.get_executor(), 
                            anet::end_point_wrapper{port, io__::ip::tcp::v4()})); // ex new
        }

    private:
        void accept_connections(anet::tcp_listener_ptr &&listener) noexcept
        {
#ifdef SERVER_ENABLE_HANDLER_TRACKING
            //std::cout << " @server=" << this << ": @listens " 
                    //<< listener->socketData_->socket_. << '\n';
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
                auto newSocket = std::make_shared<anet::socket_data>(pool_.get_executor());
                std::swap(newSocket, listener->socketData_);

#ifdef SERVER_ENABLE_HANDLER_TRACKING
                std::cout << " @server=" << this << ": @connect " << newSocket->socket_.remote_endpoint() << '\n';
#endif
                connectedIP[newSocket] = newSocket->socket_.remote_endpoint().address().to_string();
                io__::post(pool_.get_executor(), std::bind(chatLobby::authorization, &lobby_, newSocket));
                accept_connections(std::move(listener));
            }
        }
    
    public:
        ~server_control_block()
        {
            for(auto& [socket, ip] : connectedIP)
                socket->shutdown();
            pool_.stop();
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