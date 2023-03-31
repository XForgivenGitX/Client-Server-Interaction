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
    struct users_database : virtual public boost::noncopyable
    {
        typedef std::map<common::name_t, server::IChat_member_ptr> users_tree;
        typedef std::unordered_set<anet::socket_data_ptr> active_sockets_t;
    
    private:
        users_tree userData_;
        active_sockets_t activeSockets_;
        mutable std::shared_mutex userDataMut, activeSocketsMut;

    public:
        std::optional<server::IChat_member_ptr> check_user_data(const common::name_t& name, const common::pass_t& pass) const;
        bool find_name(const common::name_t& name) const;
    
    public:
        void insert_user(server::IChat_member_ptr user);
        void erase_user(const common::name_t &user);
        void insert_socket(const anet::socket_data_ptr& socketData);
        void erase_socket(const anet::socket_data_ptr& socketData);
        void for_each_socket(std::function<void(const anet::socket_data_ptr&)> func);
    };

    
    struct channels_database : virtual public boost::noncopyable
    {
        typedef std::unordered_map<common::name_t, server::IChannel_ptr> channels;
    
    private:
        channels channels_;
        mutable std::shared_mutex channelsMut;

    public:
        std::optional<server::IChannel_ptr> find_channel(const common::name_t& name);
    
    public:
        void insert_channel(const common::name_t& name, server::IChannel_ptr new_channel);
        void erase_channel(const common::name_t& name);
    };

    struct channel_database : public boost::noncopyable
    {
        typedef std::unordered_map<anet::socket_data_ptr, server::IChat_member_ptr> active_members_t;

    public:
        active_members_t activeSockets_;
        std::deque<common::msg_type> channelHistory_;
        mutable std::shared_mutex activeSocketsMut;

        void for_each_member(std::function<void(server::channelMember&)> func);
    
    public:
        void insert_socket(anet::socket_data_ptr& socketData, server::IChat_member_ptr userDesc);
        void erase_socket(const anet::socket_data_ptr& socketData);
    };
}