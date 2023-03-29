#pragma once
#include <server_module.hpp>

namespace db
{
    struct user_input_data
    {
        name_t name_;
        pass_t pass_;
        user_input_data(const name_t& name, const pass_t& pass);
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
        bool is_register_user(const ip_type &ip) const;
        bool is_contains_user_name(const name_t &name) const;
        bool is_valid_user_data(const ip_type &ip, const name_t &name, const pass_t &pass) const;

    public:
        void add_user(const ip_type &ip, user_input_data&& user);
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
        bool is_contains_room_name(const name_t &name) const;
        server::chat_room_ptr get_chat_room_ptr(const name_t &name) const; 
        ip_type get_active_ip(const anet::socket_data_ptr& socketData) const;
        name_t get_active_name(const anet::socket_data_ptr& socketData) const;
       
    public:
        void shutdown_all_sockets();
        void insert_active_ip(const anet::socket_data_ptr& socketData, const ip_type& ip);
        void erase_active_ip(const anet::socket_data_ptr& socketData);
        void insert_active_name(const anet::socket_data_ptr& socketData, const name_t& name);
        void erase_active_name(const anet::socket_data_ptr& socketData);
        void insert_room(const server::chat_room_ptr& room, const name_t& name);
       
    };
}