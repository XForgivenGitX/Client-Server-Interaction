#pragma once

#define BOOST_ASIO_DISABLE_HANDLER_TYPE_REQUIREMENTS
#define BOOST_ASIO_NO_DEPRECATED
//#define BOOST_ASIO_ENABLE_HANDLER_TRACKING
#define BOOST_ASIO_DEFAULT_LINKED_LIBS
#define BOOST_NO_ANSI_APIS 
//#define NETWORK_ENABLE_HANDLER_TRACKING

#include <utility.hpp>
#include <socket_data.hpp>
#include <send_receive.hpp>
#include <listen_connect.hpp>