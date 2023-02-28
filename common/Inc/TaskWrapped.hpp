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
    
    template <typename Func> 
    TaskWrapped<Func> make_task_wrapped(const Func&);

    template <class T>
    struct TaskWrappedWithConnections
    {
        server::ConnectionDataPtr data_;
        T taskUnwrapped_;
        explicit TaskWrappedWithConnections(server::ConnectionDataPtr&& data, T& taskUnwrapped);
        void operator()(const boost::system::error_code& error, std::size_t bytes_transferred);
    };
}
