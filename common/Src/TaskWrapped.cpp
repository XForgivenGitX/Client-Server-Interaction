#include "ClientServer.hpp"

template <typename T>
task::TaskWrapped<T>::TaskWrapped(const T &func) : taskUnwrapped_(func)
{}

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
        std::cerr << "*unknown error*\n";
    }
}