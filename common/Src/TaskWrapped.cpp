#include "ClientServer.hpp"

template <typename Func>
task::TaskWrapped<Func>::TaskWrapped(std::function<Func> &taskUnwrapped)
    : taskUnwrapped_(taskUnwrapped.target()) {}


template <typename Func>
template <typename... Args>
void task::TaskWrapped<Func>::operator()(Args &&...args)
{
    try
    {
        boost::this_thread::interruption_point();
    }
    catch (const boost::thread_interrupted &)
    {
        std::cerr << "thread interruption" << std::endl;
    }
    try
    {
        taskUnwrapped_(std::forward(args)...);
    }
    catch (const std::exception &ex)
    {
        std::cerr << ex.what << std::endl;
    }
    catch (const boost::thread_interrupted &)
    {
        std::cerr << "thread interruption" << std::endl;
    }
    catch (...)
    {
        std::cerr << "unknown error when calling the function:\n"
                  << boost::typeindex::type_id_with_cvr<Func>() << std::endl;
    }
}