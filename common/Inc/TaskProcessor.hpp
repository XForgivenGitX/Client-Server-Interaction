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
    
    private:
        static server::ConnectionDataPtr create_connection(const std::string& address, const unsigned short port_num);
        template <typename Func>
	    static void async_write_data(server::ConnectionDataPtr&& dataPtr, const Func& func);
    
    public:
        template <typename Func>
        static void send(const std::string& address, const unsigned short port, const std::string& data, const Func& func);	
        
    };
}