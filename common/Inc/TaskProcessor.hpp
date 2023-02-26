#pragma once
#include <string>
#include <boost/asio/io_service.hpp>
//#include <boost/core/noncopyable.hpp>
//#include <boost/asio/write.hpp>

// namespace io = boost::asio;

// namespace detail
// {
//     template <typename T>
//     struct Singleton : boost::noncopyable
//     {
//         static T &get_instance()
//         {
//             static T instance;
//             return instance;
//         }

//     protected:
//         Singleton() = default;
//     };
// }

// namespace server
// {
//     class TaskProcessor : public detail::Singleton<TaskProcessor>
//     {
//         friend Singleton<TaskProcessor>;
//         TaskProcessor() = default;

//     protected:
//         static io::io_service &get_ios()
//         {
//             static io::io_service ios;
//             static io::io_service::work work(ios);
//             return ios;
//         }

//     public:
//         template <typename T>
//         static void push_task(const T &task_unwrapped)
//         {
//             get_ios().post(make_task_wrapped(task_unwrapped));
//         }

//         static void start() { get_ios().run(); }
//         static void stop() { get_ios().stop(); }

//     public:
//     };
// }