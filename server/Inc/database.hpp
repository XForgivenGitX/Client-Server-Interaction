#pragma once
#include <server_module.hpp>

namespace db
{
    struct user_input_data
    {
        name_t name_;
        pass_t pass_;
        user_input_data(const name_t& name, const pass_t& pass)
            : name_(name), pass_(pass){}
    };
    
    struct server_database : public boost::noncopyable
    {
        typedef std::map<ip_type, user_input_data> users_config_tree;
        typedef std::set<name_t> users_names_tree;

    private:
        users_config_tree usersConfig_{};
        users_names_tree usersNames_{};
        
    private:
        mutable std::shared_mutex usersConfigMut, usersNamesMut;

    public:
        bool is_register_user(const ip_type &ip) const
        {
            std::shared_lock lock(usersConfigMut);
            return usersConfig_.contains(ip);
        }

        bool is_contains_user_name(const name_t &name) const
        {
            std::shared_lock lock(usersNamesMut);
            return usersNames_.contains(name);
        }

        bool is_valid_user_data(const ip_type &ip, const name_t &name, const pass_t &pass) const
        {
            std::shared_lock lock(usersConfigMut);
            auto &[valid_name, valid_pass] = usersConfig_.find(ip)->second;
            return (valid_name == name) && (valid_pass == pass);
        }

    public:
        void add_user(const ip_type &ip, user_input_data&& user)
        {
#ifdef SERVER_ENABLE_HANDLER_TRACKING
            std::cout << "@database-server: insert user: " << ip << " " << user.name_ << " " << user.pass_ << '\n';
#endif
            std::scoped_lock lock(usersConfigMut, usersNamesMut);
            usersNames_.insert(user.name_);
            usersConfig_.insert(std::make_pair(ip, std::move(user)));
        }
    };

    struct lobby_database : public boost::noncopyable
    {
        typedef std::unordered_map<anet::socket_data_ptr, ip_type> active_ip_table;
        typedef std::unordered_map<anet::socket_data_ptr, name_t> active_names_table;
        typedef std::unordered_map<name_t, server::chat_room_ptr> room_table;
    
    private:
        active_ip_table activeIp_{};
        active_names_table activeNames_{};
        room_table rooms{};

    private:
        mutable std::shared_mutex activeIpMut, activeNamesMut, roomsMut;
    
    public:
        bool is_contains_room_name(const name_t &name) const
        {
            std::shared_lock lock(roomsMut);
            return rooms.contains(name);
        }
        server::chat_room_ptr get_chat_room_ptr(const name_t &name) const
        {
            std::shared_lock lock(roomsMut);
            return rooms.find(name)->second;
        }         
        ip_type get_active_ip(const anet::socket_data_ptr& socketData) const
        {
            std::shared_lock lock(activeIpMut);
            return activeIp_.find(socketData)->second;
        }
    
        name_t get_active_name(const anet::socket_data_ptr& socketData) const
        {
            std::shared_lock lock(activeNamesMut);
            return activeNames_.find(socketData)->second;
        }
    public:
        void shutdown_all_sockets()
        {
#ifdef SERVER_ENABLE_HANDLER_TRACKING
            std::cout << "@database-lobby: shutdown all sockets";
#endif
            std::lock_guard lock(activeIpMut);
            for(auto& [socket, ip] : activeIp_)
                socket->shutdown();
        }
        
        void insert_active_ip(const anet::socket_data_ptr& socketData, const ip_type& ip)
        {
#ifdef SERVER_ENABLE_HANDLER_TRACKING
            std::cout << "@database-lobby: insert active ip: " << ip << '\n';
#endif
            std::lock_guard lock(activeIpMut);
            activeIp_.insert(std::make_pair(socketData, ip));
        }

        void erase_active_ip(const anet::socket_data_ptr& socketData)
        {
#ifdef SERVER_ENABLE_HANDLER_TRACKING
            std::cout << "@database-lobby: erase active ip: " << activeIp_[socketData] << '\n';
#endif
            std::lock_guard lock(activeIpMut);
            activeIp_.erase(socketData);
        }
    
        void insert_active_name(const anet::socket_data_ptr& socketData, const name_t& name)
        {
#ifdef SERVER_ENABLE_HANDLER_TRACKING
            std::cout << "@database-lobby: insert active name: " << name << '\n';
#endif
            std::lock_guard lock(activeNamesMut);
            activeNames_.insert(std::make_pair(socketData, name));
        }

        void erase_active_name(const anet::socket_data_ptr& socketData)
        {
#ifdef SERVER_ENABLE_HANDLER_TRACKING
            std::cout << "@database-lobby: erase active name: " << activeNames_[socketData] << '\n';
#endif
            std::lock_guard lock(activeNamesMut);
            activeNames_.erase(socketData);
        }

        void insert_room(const server::chat_room_ptr& room, const name_t& name)
        {
#ifdef SERVER_ENABLE_HANDLER_TRACKING
            std::cout << "@database-lobby: insert room: " << name << '\n';
#endif
            std::lock_guard lock(roomsMut);
            rooms.insert(std::make_pair(name, room));
        }
    };
}