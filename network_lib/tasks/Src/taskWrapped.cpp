#include <ClientServer.hpp>

// template <typename Func, typename Result>
// task::TaskWrapped<Func, Result>::TaskWrapped(std::function<Func> &taskUnwrapped) : taskUnwrapped_(taskUnwrapped)
// {
// }
// template <typename Func, typename Result>
// task::TaskWrapped<Func, Result>::TaskWrapped(const std::function<Func> &&taskUnwrapped) : taskUnwrapped_(std::move(taskUnwrapped))
// {
// }

// template <typename Func, typename Result>
// template <typename... Args>
// std::optional<Result> task::TaskWrapped<Func, Result>::operator()(Args &&...args) const noexcept
// {
//     Result returnVal;
//     try
//     {
//         boost::this_thread::interruption_point();
//     }
//     catch (const boost::thread_interrupted &)
//     {
//         std::cerr << "thread interruption" << std::endl;
//     }
//     try
//     {
//         returnVal = taskUnwrapped_(std::forward<Args>(args)...);
//     }
//     catch (const std::exception &ex)
//     {
//         std::cerr << ex.what() << std::endl;
//         return std::nullopt;
//     }
//     catch (const boost::thread_interrupted &)
//     {
//         std::cerr << "thread interruption" << std::endl;
//         return std::nullopt;
//     }
//     catch (...)
//     {
//         std::cerr << "unknown error when calling the function:\n"
//                   << boost::typeindex::type_id_with_cvr<Func>() << std::endl;
//         return std::nullopt;
//     }
//     return returnVal;
// }

// template <typename Func>
// task::TaskWrapped<Func> make_task_wrapped(const std::function<Func> &&func)
// {
//     return TaskWrapped(std::move(func));
// }