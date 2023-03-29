#pragma once
#include <memory>
#include <algorithm>
#include <boost/serialization/singleton.hpp>
#include "send_receive.hpp"
#include "protocol.hpp"
#include "logger.hpp"
#include "database.hpp"
#include "interface.hpp"
#include "chat_member.hpp"
#include "channel.hpp"
namespace server
{
    struct connections_manager :    
        public IConnection, 
        private db::users_database,
        public db::channels_database,
        public std::enable_shared_from_this<connections_manager>
    {
        using cmd_type = common::command;

    public:
        void enter_user(anet::socket_data_ptr&) override;
        void leave_user(anet::socket_data_ptr&) override;
        void leave_all() override;

        bool add_channel(const common::name_t& roomName) override
        {
            if(!find_channel(roomName))
            {
                insert_channel(roomName, std::make_shared<channel>());
                return true;
            }
            else return false;
        }
        
        bool join_channel(const common::name_t& roomName, anet::socket_data_ptr& socketData, IChat_member_ptr member)
        {
            if(auto channelOpt = find_channel(roomName); channelOpt)
            {
                channelOpt.value()->enter_user(socketData, member);
            }
        }

    private:
        void send_command(anet::socket_data_ptr&, cmd_type cmd);
        void send_command_handler(anet::socket_data_ptr&, const anet::err_c& error);
        void receive_command_handler(anet::socket_data_ptr& , const anet::err_c& error);
    };
    typedef std::shared_ptr<connections_manager> connections_manager_ptr;
}