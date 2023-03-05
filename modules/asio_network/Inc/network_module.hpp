#pragma once

#include <iostream>
#include <functional>
#include <string>

/////////////////////////////////////
#define BOOST_ASIO_DISABLE_HANDLER_TYPE_REQUIREMENTS
#define BOOST_ASIO_NO_DEPRECATED

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

/////////////////////////////////////
//first headers
#include <network/utility.hpp>
#include <network/socket_data.hpp>

//second headers
#include <network/send_receive.hpp>
#include <network/listen_connect.hpp>
