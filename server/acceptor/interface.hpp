#pragma once
#include <memory>
#include "socket_data.hpp"
#include "protocol.hpp"

namespace server
{
    struct IChat_user
    {
    protected:
        common::name_t name_;
        common::pass_t pass_;
        
    public:
        IChat_user(const common::name_t& name, const common::pass_t& pass)
            : name_(name), pass_(pass) {}
        virtual ~IChat_user() = default;
        
    public:
        virtual common::name_t get_pass() const = 0;
        virtual common::pass_t get_name() const = 0;
    };
    typedef std::shared_ptr<IChat_user> IChat_user_ptr;
     
    struct IConnections_manager 
    {
        virtual void enter_user(const anet::socket_data_ptr &) = 0;
        virtual void leave_user(const anet::socket_data_ptr &) = 0;
        virtual void leave_everyone() = 0;
        
        virtual void send_command(const anet::socket_data_ptr &, common::command) = 0;
        virtual ~IConnections_manager() = default;
    };
    typedef std::shared_ptr<IConnections_manager> IConnections_manager_ptr;
    
    struct IChannel
    {
    protected:
        IConnections_manager_ptr connectionManager_;
        
    public:
        IChannel(const IConnections_manager_ptr& connectionManager)
            : connectionManager_(connectionManager) {}
        virtual ~IChannel() = default;
    
    public:
        virtual void enter_user(const anet::socket_data_ptr &, const IChat_user_ptr&) = 0;
        
        virtual void normal_leave_user(const anet::socket_data_ptr &) = 0;
        virtual void forced_leave_user(const anet::socket_data_ptr &) = 0;
        
        virtual void leave_everyone() = 0;
        virtual void send_everyone(const common::msg_type&) = 0;
        
        virtual void send_history(const anet::socket_data_ptr& socketData) = 0;
    };
    typedef std::shared_ptr<IChannel> IChannel_ptr;

    struct IChannel_member
    {
    protected:
        IChannel_ptr myChannel_;
        IChat_user_ptr myData_;
        
    public:
        IChannel_member(const IChannel_ptr& myChannel, const IChat_user_ptr& myData)
            : myChannel_(myChannel), myData_(myData) {}

    public:
        virtual void send_command_handler(anet::socket_data_ptr& socketData, const anet::err_c& error_c) = 0;
        virtual void start_receiving_messages(const anet::socket_data_ptr& socketData) = 0;
    };
    typedef std::unique_ptr<IChannel_member> IChannel_member_ptr;
}
