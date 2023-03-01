#pragma once
#include "ClientServer.hpp"
namespace task
{
    template <typename Func>
    struct TaskWrapped
    {
        Func taskUnwrapped_;
        explicit TaskWrapped(std::function<Func>& taskUnwrapped);
        
        template <typename... Args>
        void operator()(Args &&...args);
    };
    

}
