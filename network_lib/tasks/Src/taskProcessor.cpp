#include <ClientServer.hpp>

io__::io_service &detail::TaskProcessor::get_ios()
{
    static io__::io_service ios;
    static io__::io_service::work work(ios);
    return ios;
}

template <typename T>
void detail::TaskProcessor::push_task(const T &task_unwrapped)
{
    //get_ios().post(task::make_task_wrapped(task_unwrapped));
}

void detail::TaskProcessor::start()
{
    get_ios().run();
}

void detail::TaskProcessor::stop()
{
    get_ios().stop();
}