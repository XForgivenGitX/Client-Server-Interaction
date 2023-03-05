#pragma once
#include <network_module.hpp>

namespace server
{
    void send_handler(anet::socket_data_ptr &&socketData, const boost::system::error_code &error);
    void receive_handler(anet::socket_data_ptr &&socketData, const boost::system::error_code &error);
    void handler_accepted_connection(anet::tcp_listener_ptr &&listener, const boost::system::error_code &error);
}
