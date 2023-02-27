#pragma once
#include "ClientServer.hpp"

namespace detail
{
    template <typename T>
    struct Singleton : private boost::noncopyable
    {
        static T &get_instance();
    protected:
        Singleton() = default;
    };

    class TaskProcessor : public detail::Singleton<TaskProcessor>
    {
        friend Singleton<TaskProcessor>;
        TaskProcessor() = default;

    protected:
        static io__::io_service &get_ios();
        
    public:
        template <typename T> static void push_task(const T &task_unwrapped);
        static void start();
        static void stop();

    public:
        static std::unique_ptr<server::ConnectionData> create_connection(std::string address, unsigned short port_num);
    };
}