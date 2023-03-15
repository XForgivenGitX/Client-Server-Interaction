#pragma once
#include <unordered_map>
#include <shared_mutex>

/////////////////////////////////
#define BOOST_THREAD_PROVIDES_THREAD_DESTRUCTOR_CALLS_TERMINATE_IF_JOINABLE

/////////////////////////////////
#include <network_module.hpp>
#include <boost/asio/executor_work_guard.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread/thread.hpp>

/////////////////////////////////
#define SERVER_MODULE_DEBUG

/////////////////////////////////
#include <chat_room.hpp>
#include <chat_user.hpp>
//__
#include <server_session.hpp>

////////////////////////////////
typedef io__::executor_work_guard<io__::io_context> worker_t;
typedef std::list<std::shared_ptr<server::server_session>> ptrs_to_servers;
typedef std::list<std::shared_ptr<worker_t>> ptrs_to_workers;