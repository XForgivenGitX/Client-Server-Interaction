#pragma once
#include "ClientServer.hpp"
namespace task
{
    template <class T>
    struct TaskWrapped
    {
        T taskUnwrapped_;
        explicit TaskWrapped(const T &func);
        void operator()() const;
    };
}
