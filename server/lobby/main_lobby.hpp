#pragma once
#include <memory>
#include <algorithm>
#include "send_receive.hpp"
#include "connections.hpp"
#include "user_account.hpp"
#include "protocol.hpp"
#include "logger.hpp"
#include "channel.hpp"

namespace server
{
    struct main_lobby :    
        public ILobby, 
        private db::lobby_database,
        public boost::noncopyable, 
        public std::enable_shared_from_this<main_lobby>
    {
        using cmd_type = common::command;
        using db::lobby_database::insert_channel;
        using db::lobby_database::erase_channel;
        using db::lobby_database::find_channel;

    public:
        void enter_user(anet::socket_data_ptr&) override;
        void leave_user(anet::socket_data_ptr&) override;
        bool add_channel(const common::name_t& name) override
        {
            if(find_channel(name))
            {
                
                return true;
            }
            else
            {
                insert_channel(name, std::make_shared<channel>());
                return true;
            }
        }
        
        join_channel_v join_channel(const common::name_t& name, anet::socket_data_ptr& socketData) override
        {
            if(auto channelOpt = find_channel(name); channelOpt)
            {
                channelOpt.value()->enter_user(socketData);
                return channelOpt;
            }
            else
            {
                return std::nullopt;;
            }
        }
        
        void leave_all() override;

    private:
        void send_command(anet::socket_data_ptr&, cmd_type cmd);
        void send_command_handler(anet::socket_data_ptr&, const anet::err_c& error);
        void receive_command_handler(anet::socket_data_ptr& , const anet::err_c& error);
    };
    typedef std::shared_ptr<main_lobby> main_lobby_ptr;
}