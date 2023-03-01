#pragma once
#include <ClientServer.hpp>

namespace detail
{
    struct TaskProcessor
    {
    public:
        static io__::io_service &get_ios();
        TaskProcessor() = delete;
    
    public:
        template <typename T> static void push_task(const T &task_unwrapped);
        static void start();
        static void stop();
    };



}