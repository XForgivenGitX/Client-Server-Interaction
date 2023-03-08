#pragma once

#include <memory>
#include <optional>
#include <functional>

#include <boost/function_types/result_type.hpp>
#include <boost/type_index.hpp>
#include <boost/thread/thread.hpp>

namespace utility::detail
{
    struct dummy{};

    template <typename Res>
    std::optional<Res> operator,(Res&& res, std::optional<dummy>) 
    {
        return std::forward<Res>(res);
    }
}

namespace utility
{
    struct bad_socket : public std::exception
    {
        virtual const char *what() const noexcept
        {
            return "socket is closed, read write is not possible";
        }
        virtual ~bad_socket() = default;
    };

//_____
    
    template <class T, class... Args>
    std::unique_ptr<T> safe_make_unique(Args &&...args) noexcept
    {
        try
        {
            return std::make_unique<T>(std::forward<Args>(args)...);
        }
        catch (const std::bad_alloc &ex)
        {
            std::cerr << ex.what() << '\n';
#ifdef DEBUG__
            assert(false);
#endif // DEBUG__
        }
        catch (const std::exception &ex)
        {
            std::cerr << ex.what() << '\n';
        }
        catch (...)
        {
            std::cerr << "unknown error when allocating memory for the type:"
                      << boost::typeindex::type_id<T>();
        }
        return nullptr;
    }

//_____
    
    template <class Func>
    struct task_wrapped
    {
        const std::function<Func> taskUnwrapped_;

        explicit task_wrapped(std::function<Func> &taskUnwrapped) : taskUnwrapped_(taskUnwrapped)
        {
        }
        explicit task_wrapped(std::function<Func> &&taskUnwrapped) : taskUnwrapped_(std::move(taskUnwrapped))
        {
        }

        template <typename... Args>
        auto operator()(Args &&...args) const noexcept
        {
            using Ret = decltype(taskUnwrapped_(std::forward<Args>(args)...), std::optional<detail::dummy>());
            try
            {
                boost::this_thread::interruption_point();
            }
            catch (const boost::thread_interrupted &)
            {
                std::cerr << "thread interruption" << '\n';
            }
            try
            {
                return taskUnwrapped_(std::forward<Args>(args)...), std::optional<detail::dummy>();
            }
            catch (const std::exception &ex)
            {
                std::cerr << ex.what() << '\n';
            }
            catch (const boost::thread_interrupted &)
            {
                std::cerr << "thread interruption" << '\n';
            }
            catch (...)
            {
                std::cerr << "unknown error when calling the function: "
                          << boost::typeindex::type_id<Func>() << '\n';
            }
            return Ret();
        }
    };

    template <typename Func>
    task_wrapped<Func> make_task_wrapped(std::function<Func> &&func)
    {
        return task_wrapped(std::move(func));
    }

    template <typename Func>
    task_wrapped<Func> make_task_wrapped(std::function<Func> &func)
    {
        return task_wrapped(func);
    }
}