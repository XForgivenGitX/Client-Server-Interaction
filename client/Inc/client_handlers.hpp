#pragma once
#include <network_module.hpp>

namespace client
{

void send_handler(anet::socket_data_ptr&& socketData, const boost::system::error_code& error);

void receive_handler(anet::socket_data_ptr&& socketData, const boost::system::error_code& error);
void connection_handler(anet::socket_data_endpoint_ptr &&socketData, const boost::system::error_code &);
}