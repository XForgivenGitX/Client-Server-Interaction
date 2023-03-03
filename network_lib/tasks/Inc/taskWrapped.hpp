#pragma once
#include <ClientServer.hpp>

// TODO
// 1.does not instance void (result)
// 2.delete the wrapper in the form of function<>
// 3.comma operator

namespace task
{
    namespace detail
    {
        struct dummy{};

        template <typename Res>
        constexpr bool task_wrapped_requires()
        {
            if(std::is_void_v<Res>) return true;
            else return std::is_default_constructible_v<Res> &&
                   std::is_nothrow_copy_constructible_v<Res> &&
                   std::is_nothrow_move_constructible_v<Res>;
        }

        template <class Res>
        struct my_result
        {
            using type = std::optional<Res>;
        };

        template <>
        struct my_result<void>
        {
            using type = std::optional<dummy>;
        };

        template <typename Res>
        using my_result_t = my_result<Res>::type;

    }

    template <typename Func, typename Res = typename boost::function_types::result_type<Func>::type>
        requires(detail::task_wrapped_requires<Res>())
    struct TaskWrapped
    {
        const std::function<Func> taskUnwrapped_;

        explicit TaskWrapped(std::function<Func> &taskUnwrapped) : taskUnwrapped_(taskUnwrapped)
        {
        }
        explicit TaskWrapped(std::function<Func> &&taskUnwrapped) : taskUnwrapped_(std::move(taskUnwrapped))
        {
        }

        template <typename... Args>
        detail::my_result_t<Res> operator()(Args &&...args) const noexcept
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
                if constexpr (std::is_void_v<Res>)
                {
                    taskUnwrapped_(std::forward<Args>(args)...);
                    return detail::dummy{};
                }
                else
                {
                    return taskUnwrapped_(std::forward<Args>(args)...);
                }
            }
            catch (const std::exception &ex)
            {
                std::cerr << ex.what() << std::endl;
            }
            catch (const boost::thread_interrupted &)
            {
                std::cerr << "thread interruption" << std::endl;
            }
            catch (...)
            {
                std::cerr << "unknown error when calling the function: "
                          << boost::typeindex::type_id<Func>() << std::endl;
            }
            return std::nullopt;
        }
    };

    template <typename Func>
    TaskWrapped<Func> make_task_wrapped(std::function<Func> &&func)
    {
        return TaskWrapped(std::move(func));
    }

    template <typename Func>
    TaskWrapped<Func> make_task_wrapped(std::function<Func> &func)
    {
        return TaskWrapped(func);
    }
}
