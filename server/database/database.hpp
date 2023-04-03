#pragma once

#include <unordered_map>
#include <unordered_set>
#include <map>
#include <deque>
#include <optional>
#include <shared_mutex>
#include <boost/noncopyable.hpp>

#include "socket_data.hpp"
#include "logger.hpp"
#include "interface.hpp"

namespace db
{
    struct users_database
    {
        typedef std::map<common::name_t, server::IChat_user_ptr> 
            users_tree;
        
        typedef std::unordered_map<anet::socket_data_ptr, server::IChat_user_ptr> 
            active_sockets_t;
        
        typedef std::function<void(const active_sockets_t::value_type&)> 
            user_function_t;
        
        typedef std::optional<server::IChat_user_ptr>  
            find_user_data_ret_v;
    
    private:
        users_tree userData_;
        active_sockets_t activeSockets_;
        mutable std::shared_mutex userDataMut, activeSocketsMut;

    public:
        find_user_data_ret_v find_user_data(const common::name_t& name, const common::pass_t& pass) const;
        bool is_user_name(const common::name_t& name) const;
        
    public:
        void insert_socket(const anet::socket_data_ptr& socketData, server::IChat_user_ptr = nullptr);
        void erase_socket(const anet::socket_data_ptr& socketData);
        void for_each_socket(user_function_t func);
        server::IChat_user_ptr get_active_user_data(const anet::socket_data_ptr& socketData);  
        
        void insert_user(server::IChat_user_ptr user);
        void erase_user(const common::name_t &user);
        
    };

    struct channels_database
    {
        typedef std::unordered_map<common::name_t, server::IChannel_ptr> 
            channels;
        
        typedef std::optional<server::IChannel_ptr> 
            find_channel_ret_v;

    private:
        channels channels_;
        mutable std::shared_mutex channelsMut;

    public:
        find_channel_ret_v find_channel(const common::name_t& name);
        bool is_channel_name(const common::name_t& name);
    
    public:
        void insert_channel(const common::name_t& name, server::IChannel_ptr new_channel);
        void erase_channel(const common::name_t& name);
    };

    struct channel_database
    {
        typedef std::unordered_map<anet::socket_data_ptr, server::IChannel_member_ptr> 
            active_members_t;
        
        typedef std::deque<common::msg_type> 
            channel_history_t;
        
        typedef std::function<void(const active_members_t::value_type&)>
            active_members_func;

        typedef std::function<void(const channel_history_t::value_type&)>
            channel_history_func;
    
    private:
        active_members_t activeSockets_;
        channel_history_t channelHistory_;
        mutable std::shared_mutex activeSocketsMut, channelHistoryMut;
    
    public:
        active_members_t::const_iterator get_channel_member(const anet::socket_data_ptr& socketData);
        void insert_chat_member(const anet::socket_data_ptr& socketData, server::IChannel_member_ptr&& channelMember);
        void erase_chat_member(const anet::socket_data_ptr& socketData);
        void for_each_member(active_members_func func);
        void for_each_history(channel_history_func func);
        void add_message_to_history(const common::msg_type& msg);
    };
}