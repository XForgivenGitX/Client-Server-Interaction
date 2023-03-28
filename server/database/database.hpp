#pragma once

#include <unordered_map>
#include <unordered_set>
#include <map>
#include <memory>
#include <optional>
#include <shared_mutex>
#include <boost/noncopyable.hpp>

#include "socket_data.hpp"
#include "logger.hpp"
#include "i_channels.hpp"

namespace db
{
    
    
    struct server_database : public boost::noncopyable
    {
        typedef std::map<common::name_t, server::Iuser_ptr> users_tree;
        typedef std::unordered_set<anet::socket_data_ptr> activeSockets;
    
    private:
        users_tree userData_;
        activeSockets activeSockets_;
    private:
        mutable std::shared_mutex userDataMut, activeSocketsMut;

    public:
        std::optional<server::Iuser_ptr> check_user_data(const common::name_t& name, const common::pass_t& pass) const
        {
            std::shared_lock lock(userDataMut);
            auto it = userData_.find(name);
            return ((it != userData_.end() && (it->second->pass_ == pass)) ? 
                            std::optional<server::Iuser_ptr>(it->second) : std::nullopt);
        }

        bool find_name(const common::name_t& name) const
        {
            return userData_.find(name) != userData_.end();
        }
    public:
        server::Iuser_ptr insert_user(server::Iuser_ptr user)
        {
            std::unique_lock lock(userDataMut);
            auto ret_val = userData_.insert(std::make_pair(user->name_, user));
            lock.unlock();
#ifdef SERVER_ENABLE_HANDLER_TRACKING
            BOOST_LOG_TRIVIAL(DB_LOG_SEVERITY) 
                << lg::build_log("database-server: insert user", 
                "name: " + user->name_,
                "pass: " + user->pass_,
                "status: " + lg::boolalpha_cast(ret_val.second));
#endif
            if(!ret_val.second) 
                throw std::runtime_error("error adding a new user to the chat");
            return ret_val.first->second;
        }
        
        void erase_user(const common::name_t &user)
        {
#ifdef SERVER_ENABLE_HANDLER_TRACKING
            BOOST_LOG_TRIVIAL(DB_LOG_SEVERITY) 
                << lg::build_log("database-server: erase user", 
                "name: " + user);
#endif
            std::lock_guard lock(userDataMut);
            userData_.erase(user);
        }
        void insert_socket(const anet::socket_data_ptr& socketData)
        {
            std::lock_guard lock(activeSocketsMut);
            activeSockets_.insert(socketData);
        }
        void erase_socket(const anet::socket_data_ptr& socketData)
        {
            std::lock_guard lock(activeSocketsMut);
            activeSockets_.erase(socketData);
        }
    };

//     typedef std::unordered_map<common::name_t, server::IChannel_ptr> channels;
//     typedef channels::const_iterator channels_it;
    
//     struct lobby_database
//     {
//     private:
//         channels channels_;

//     private:
//         mutable std::shared_mutex channelsMut;

//     public:
//         std::optional<> find_channel(const common::name_t& name)
//         {
//             std::shared_lock lock(channelsMut);
//             return channels_.find(name) != channels_.end();
//         }
    
//     public:
//         void insert_channel(const common::name_t& name, const server::IChannel_ptr& new_channel)
//         {
//             std::unique_lock lock(channelsMut);
//             auto ret_val = channels_.insert(std::make_pair(name, new_channel));
//             lock.unlock();
// #ifdef SERVER_ENABLE_HANDLER_TRACKING
//             BOOST_LOG_TRIVIAL(DB_LOG_SEVERITY) 
//                 << lg::build_log("database-lobby: insert channel", 
//                 "name: " + name,
//                 "status: " + lg::boolalpha_cast(ret_val.second));
// #endif
            
//         }

//         void erase_channel(const common::name_t& name)
//         {
// #ifdef SERVER_ENABLE_HANDLER_TRACKING
//             BOOST_LOG_TRIVIAL(DB_LOG_SEVERITY) 
//                 << lg::build_log("database-lobby: erase channel", 
//                 "name: " + name);
// #endif
//             std::lock_guard lock(channelsMut);
//             channels_.erase(name);
//         }


//     };

}
    
    
    
    
    // struct lobby_database : public boost::noncopyable
    // {
    //     typedef std::unordered_map<anet::socket_data_ptr, ip_type> active_ip_table;
    //     typedef std::unordered_map<anet::socket_data_ptr, name_t> active_names_table;
    //     typedef std::unordered_map<name_t, server::chat_room_ptr> room_table;

    // private:
    //     active_ip_table activeIp_{};
    //     active_names_table activeNames_{};
    //     room_table rooms{};

    // private:
    //     mutable std::shared_mutex activeIpMut, activeNamesMut, roomsMut;

    // public:
    //     bool is_contains_room_name(const name_t &name) const;
    //     server::chat_room_ptr get_chat_room_ptr(const name_t &name) const;
    //     ip_type get_active_ip(const anet::socket_data_ptr& socketData) const;
    //     name_t get_active_name(const anet::socket_data_ptr& socketData) const;

    // public:
    //     void shutdown_all_sockets();
    //     void insert_active_ip(const anet::socket_data_ptr& socketData, const ip_type& ip);
    //     void erase_active_ip(const anet::socket_data_ptr& socketData);
    //     void insert_active_name(const anet::socket_data_ptr& socketData, const name_t& name);
    //     void erase_active_name(const anet::socket_data_ptr& socketData);
    //     void insert_room(const server::chat_room_ptr& room, const name_t& name);

    // };