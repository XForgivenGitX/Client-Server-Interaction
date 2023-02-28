#include "ClientServer.hpp"

template <typename T>
task::TaskWrapped<T>::TaskWrapped(const T &func) : taskUnwrapped_(func)
{
}

template <typename T>
void task::TaskWrapped<T>::operator()() const
{
    try
    {
        boost::this_thread::interruption_point();
    }
    catch (const boost::thread_interrupted &)
    {
        std::cerr << "THREAD INTERRUPTION\n";
    }
    try
    {
        taskUnwrapped_();
    }
    catch (const std::exception &ex)
    {
        std::cerr << "EXCEPTION: " << ex.what << '\n';
    }
    catch (const boost::thread_interrupted &)
    {
        std::cerr << "THREAD INTERRUPTION\n";
    }
    catch (...)
    {
        std::cerr << "UNKNOWN ERROR\n";
    }
}

template <typename Func>
task::TaskWrapped<Func> task::make_task_wrapped(const Func &f)
{
    return task::TaskWrapped<Func>(f);
}

template <typename T>
task::TaskWrappedWithConnections<T>::TaskWrappedWithConnections(server::ConnectionDataPtr &&data, T &taskUnwrapped)
    : data_(std::move(data)), taskUnwrapped_(taskUnwrapped)
{
}

template <typename T>
void task::TaskWrappedWithConnections<T>::operator()(const boost::system::error_code &error, std::size_t bytes_transferred)
{
    data_->data.resize(bytes_transferred);
    taskUnwrapped_(error, std::move(data_));
}