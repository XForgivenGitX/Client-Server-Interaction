#pragma once
#include <memory>
#include "socket_data.hpp"
#include "protocol.hpp"

namespace server
{
    struct IChat_member
    {
        common::name_t name_;
        common::pass_t pass_;
        IChat_member(const common::name_t& name, const common::pass_t& pass)
            : name_(name), pass_(pass) {}
        virtual void send_command(anet::socket_data_ptr&, common::command) = 0;
        virtual ~IChat_member() = default;
    };
    typedef std::shared_ptr<IChat_member> IChat_member_ptr;
    typedef std::pair<const anet::socket_data_ptr, server::IChat_member_ptr> channelMember;

    struct IChannel
    {
        virtual void send_all(const common::msg_type&) = 0;
        virtual void enter_user(anet::socket_data_ptr &, IChat_member_ptr) = 0;
        virtual void leave_user(channelMember &) = 0;
        virtual void leave_all() = 0;
        virtual ~IChannel() = default;
    };
    typedef std::shared_ptr<IChannel> IChannel_ptr;

    struct IConnection
    {
        typedef std::optional<server::IChannel_ptr> join_channel_v;
        
        virtual void enter_user(anet::socket_data_ptr &) = 0;
        virtual bool add_channel(const common::name_t& name) = 0;
        virtual bool join_channel(const common::name_t& name, anet::socket_data_ptr &, IChat_member_ptr) = 0;
        virtual void leave_user(anet::socket_data_ptr &) = 0;
        virtual void leave_all() = 0;
        virtual ~IConnection() = default;
    };
    typedef std::shared_ptr<IConnection> IConnection_ptr;
}
