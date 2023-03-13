#pragma once

#include <iostream>
#include <functional>
#include <string>

/////////////////////////////////////
#define BOOST_ASIO_DISABLE_HANDLER_TYPE_REQUIREMENTS
#define BOOST_ASIO_NO_DEPRECATED
//#define BOOST_ASIO_ENABLE_HANDLER_TRACKING
#define BOOST_ASIO_DEFAULT_LINKED_LIBS

/////////////////////////////////////
#include <boost/system/error_code.hpp>
#include <boost/asio/write.hpp>
#include <boost/asio/read.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/core/noncopyable.hpp>

/////////////////////////////////////
namespace{ namespace io__ = boost::asio;}
typedef std::string data_t;
enum class tags
{
    send_tag, receive_tag
};

#define FRAME_MAX_LENGHT  128
#define FRAME_MIN_LENGHT  1
//#define NETWORK_MODULE_DEBUG
//#define NETWORK_MODULE_RELEASE

/////////////////////////////////////
//first headers
#include <network/utility.hpp>
#include <network/socket_data.hpp>

//second headers
#include <network/send_receive.hpp>
#include <network/listen_connect.hpp>
