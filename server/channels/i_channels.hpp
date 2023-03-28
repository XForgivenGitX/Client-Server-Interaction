#pragma once
#include <deque>
#include <unordered_map>
#include "socket_data.hpp"
#include "protocol.hpp"
namespace server
{
    typedef std::string msg_type;
    typedef std::deque<msg_type> history_msg_t;
    typedef std::unordered_map<>
   
    struct user_data_for_access
    {
        name_t name_;
        pass_t pass_;
        user_data_for_access(const name_t& name, const pass_t pass)
            : name_(name), pass_(pass){}
    };
    
    struct user_info : public user_data_for_access
    {
    public:
        std::unordered_map<>
    
    public:
        user_data(const name_t &name, const pass_t &pass);
    };
    
    struct ILobby
    {
        virtual void enter_user(anet::socket_data_ptr &) = 0;
        virtual void send_command(anet::socket_data_ptr &, common::command cmd) = 0;
        virtual void leave_user(anet::socket_data_ptr &) = 0;
        virtual ~ILobby() = default;
    };
    typedef std::shared_ptr<ILobby> ILobby_ptr;

    struct IChannel
    {
        ILobby_ptr myLobby_;
        history_msg_t history_;
        std::
    };
    typedef std::shared_ptr<IChannel> IChannel_ptr;
}
