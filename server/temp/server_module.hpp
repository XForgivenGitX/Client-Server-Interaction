#pragma once
#include <unordered_map>
#include <shared_mutex>
#include <type_traits>
#include <functional>
#include <deque>
#include <map>
#include <set>

/////////////////////////////////
#define BOOST_THREAD_PROVIDES_THREAD_DESTRUCTOR_CALLS_TERMINATE_IF_JOINABLE

/////////////////////////////////
#include <network_module.hpp>
#include <protocol.hpp>
#include <boost/asio/executor_work_guard.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/asio/thread_pool.hpp>
#include <boost/serialization/singleton.hpp>
#include <boost/asio/signal_set.hpp>

/////////////////////////////////
#define SERVER_MODULE_DEBUG
#define SERVER_ENABLE_HANDLER_TRACKING

/////////////////////////////////
typedef std::string ip_type;
typedef std::string name_t;
typedef std::string pass_t;

/////////////////////////////////
#include <chat_room.hpp>
#include <database.hpp>
#include <chat_lobby.hpp>
#include <server_session.hpp>
