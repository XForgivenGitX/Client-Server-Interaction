#pragma once

#include <unordered_map>
#include <unordered_set>
#include <map>
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
        typedef std::map<common::name_t, server::IUser_ptr> users_tree;
        typedef std::unordered_set<anet::socket_data_ptr> active_sockets_t;
    
    private:
        users_tree userData_;
        active_sockets_t activeSockets_;
    
    private:
        mutable std::shared_mutex userDataMut, activeSocketsMut;

    public:
        std::optional<server::IUser_ptr> check_user_data(const common::name_t& name, const common::pass_t& pass) const
        {
            std::shared_lock lock(userDataMut);
            auto it = userData_.find(name);
            return ((it != userData_.end() && (it->second->pass_ == pass)) ? 
                            std::optional<server::IUser_ptr>(it->second) : std::nullopt);
        }

        bool find_name(const common::name_t& name) const
        {
            return userData_.find(name) != userData_.end();
        }
    public:
        void insert_user(server::IUser_ptr user)
        {
            std::unique_lock lock(userDataMut);
            auto ret_val = userData_.insert(std::make_pair(user->name_, user));
#ifdef SERVER_ENABLE_HANDLER_TRACKING
            lock.unlock();
            BOOST_LOG_TRIVIAL(DB_LOG_SEVERITY) 
                << lg::build_log("database-server: insert user", 
                "name: " + user->name_,
                "pass: " + user->pass_,
                "status: " + lg::boolalpha_cast(ret_val.second));
#endif
            if(!ret_val.second) 
                throw std::runtime_error("error adding a new user to the chat");
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

        void shutdown_all_sockets()
        {
            std::lock_guard lock(activeSocketsMut);
            for(auto& socket : activeSockets_)
                socket->shutdown();
        }
    };

    
    
    struct lobby_database : public boost::noncopyable
    {
        typedef std::unordered_map<common::name_t, server::IChannel_ptr> channels;
    
    private:
        channels channels_;

    private:
        mutable std::shared_mutex channelsMut;

    public:
        std::optional<server::IChannel_ptr> find_channel(const common::name_t& name)
        {
            std::shared_lock lock(channelsMut);
            auto it = channels_.find(name);
            if(it != channels_.end())
            {
                return it->second;
            }
            else return std::nullopt;
        }
    
    public:
        void insert_channel(const common::name_t& name, server::IChannel_ptr new_channel)
        {
            std::unique_lock lock(channelsMut);
            auto ret_val = channels_.insert(std::make_pair(name, new_channel));
#ifdef SERVER_ENABLE_HANDLER_TRACKING
            lock.unlock();
            BOOST_LOG_TRIVIAL(DB_LOG_SEVERITY) 
                << lg::build_log("database-lobby: insert channel", 
                "name: " + name,
                "status: " + lg::boolalpha_cast(ret_val.second));
#endif
            if(!ret_val.second) 
                throw std::runtime_error("error adding a new chat room");
        }

        void erase_channel(const common::name_t& name)
        {
#ifdef SERVER_ENABLE_HANDLER_TRACKING
            BOOST_LOG_TRIVIAL(DB_LOG_SEVERITY) 
                << lg::build_log("database-lobby: erase channel", 
                "name: " + name);
#endif
            std::lock_guard lock(channelsMut);
            channels_.erase(name);
        }
    };

    struct channel_database : public boost::noncopyable
    {
        typedef std::unordered_set<anet::socket_data_ptr> active_sockets_t;

    public:
        active_sockets_t activeSockets;
        mutable std::shared_mutex activeSocketsMut;

        void for_each_socket(std::function<void(anet::socket_data_ptr)> func)
        {
            std::shared_lock lock(activeSocketsMut);
            std::for_each(activeSockets.begin(), activeSockets.end(), func);
        }
    
    public:
        void insert_socket(anet::socket_data_ptr& socketData)
        {
            std::lock_guard lock(activeSocketsMut);
            if(!activeSockets.insert(socketData).second) 
                throw std::runtime_error("error adding a new chat room");
        }

        void erase_socket(anet::socket_data_ptr& socketData)
        {
            std::lock_guard lock(activeSocketsMut);
            activeSockets.erase(socketData);
        }
    };
}