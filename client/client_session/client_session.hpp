#pragma once
#include "listen_connect.hpp"
#include "send_receive.hpp"
#include "protocol.hpp"
#include <thread>
// TODO
// 1.Очищать буффер cin
using namespace common;

namespace client
{
    class client_session
    {
    public:
        client_session(io__::io_context &ios, const anet::end_point_wrapper &endPoint);
        void connection_handler(anet::socket_data_ptr &socketData, const boost::system::error_code &error);
        void send_command(anet::socket_data_ptr &socketData, common::command cmd, const common::transf_package::args_t &args = {});
        void send_command_handler(anet::socket_data_ptr &socketData, const boost::system::error_code &error);
        void authorization(anet::socket_data_ptr &socketData);
        void receive_command_handler(anet::socket_data_ptr &socketData, const boost::system::error_code &error);
        void auth_response_handler(anet::socket_data_ptr &socketData, const boost::system::error_code &error);
        void reading_and_sending_data(anet::socket_data_ptr &socketData, common::command cmd);
        void identify_and_send_command(anet::socket_data_ptr &socketData);
        void enter_to_lobby(anet::socket_data_ptr &socketData);
        void lobby_request_handler(anet::socket_data_ptr &socketData, const boost::system::error_code &error);
        void lobby_responce_handler(anet::socket_data_ptr &socketData, const boost::system::error_code &error);
        void start_communication(anet::socket_data_ptr &socketData);
        void receive_message(anet::socket_data_ptr &socketData);
        void receive_message_handler(anet::socket_data_ptr& socketData, const boost::system::error_code &error);
        void send_message_handler(anet::socket_data_ptr& socketData, const boost::system::error_code &error);
    };
}