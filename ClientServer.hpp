#pragma once
#include <string>
#include <iostream>
#include <memory>
#include <locale>
#include <functional>
#include <cassert>

#define BOOST_ASIO_DISABLE_HANDLER_TYPE_REQUIREMENTS

#include <boost/core/noncopyable.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/system/error_code.hpp>
#include <boost/thread/thread.hpp>
#include <boost/asio/write.hpp>
#include <boost/asio/read.hpp>
#include <boost/type_index.hpp>

#define io__ boost::asio

#include "ConnectionData.hpp"
#include "TaskProcessor.hpp"
#include "TaskWrapped.hpp"
#include "SendReceive.hpp"