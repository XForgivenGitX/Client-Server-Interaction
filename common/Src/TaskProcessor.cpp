#include <Inc/TaskProcessor.hpp>

template <typename T>
T &detail::Singleton<T>::get_instance()
{
    static T instance;
    return instance;
}

io::io_service &detail::TaskProcessor::get_ios()
{
    static io::io_service ios;
    static io::io_service::work work(ios);
    return ios;
}

template <typename T>
static void detail::TaskProcessor::push_task(const T &task_unwrapped)
{
    // get_ios().post(make_task_wrapped(task_unwrapped));
}

