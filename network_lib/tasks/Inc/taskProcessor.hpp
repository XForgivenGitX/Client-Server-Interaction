#pragma once
#include <ClientServer.hpp>

// TODO
//1. wrap try-catch start and stop
//2. 

namespace detail
{
    struct TaskProcessor
    {
    //private:
    public:   
        static io__::io_service &get_ios();
        TaskProcessor() = delete;
    
    public:
        template <typename T> static void push_task(const T &task_unwrapped);
        static void start();
        static void stop();
    };
}