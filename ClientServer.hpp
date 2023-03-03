#pragma once
#include <string>
#include <iostream>
#include <memory>
#include <locale>
#include <functional>
#include <cassert>
#include <type_traits>
#include <optional>
#include <algorithm>
#include <cctype>

#define BOOST_ASIO_DISABLE_HANDLER_TYPE_REQUIREMENTS

#include <boost/core/noncopyable.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/system/error_code.hpp>
#include <boost/thread/thread.hpp>
#include <boost/asio/write.hpp>
#include <boost/asio/read.hpp>
#include <boost/type_index.hpp>
#include <boost/function_types/result_type.hpp>

#define io__ boost::asio

#include "socketData.hpp"
#include "taskProcessor.hpp"
#include "sendReceive.hpp"
#include "taskWrapped.hpp"
#include "listen.hpp"
#include "connection.hpp"