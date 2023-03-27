#pragma once

#include "socket_data.hpp"
#include "protocol.hpp"
struct ILobby
{
    virtual void enter_user(anet::socket_data_ptr&) = 0;
    virtual void send_command(anet::socket_data_ptr &, common::command cmd) = 0;
    virtual void leave_user(anet::socket_data_ptr&) = 0;
    virtual ~ILobby() = default;
};
typedef std::shared_ptr<ILobby> ILobby_ptr;

