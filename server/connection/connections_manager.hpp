#pragma once
#include <memory>
#include <algorithm>
#include "send_receive.hpp"
#include "protocol.hpp"
#include "logger.hpp"
#include "database.hpp"
#include "interface.hpp"
#include "channels.hpp"

namespace server
{

    struct connections_manager :
        public IConnections_manager,
        private db::users_database,
        private db::channels_database,
        public std::enable_shared_from_this<connections_manager>
    {
        using cmd_type = common::command;

    public:
        void enter_user(const anet::socket_data_ptr &socketData) override;
        void send_command(const anet::socket_data_ptr &socketData, cmd_type cmd) override;
        void leave_user(const anet::socket_data_ptr &socketData) override;
        void leave_everyone() override;

        virtual ~connections_manager();
        
    private:
        void send_command_handler(anet::socket_data_ptr &socketData, const anet::err_c &error_c) noexcept;
        void receive_command_handler(anet::socket_data_ptr &socketData, const anet::err_c &error_c) noexcept;
    };
}