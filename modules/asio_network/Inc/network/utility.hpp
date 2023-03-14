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
    std::optional<Res> operator,(Res &&res, std::optional<dummy>)
    {
        return std::forward<Res>(res);
    }

    template <typename F>
    struct remove_function_top_const;

    template <typename R, typename... Args>
    struct remove_function_top_const<R(Args...)>
    {
        using type = R(Args...);
    };

    template <typename R, typename... Args>
    struct remove_function_top_const<R(Args...) const>
    {
        using type = R(Args...);
    };
 
    template <class F, class Obj, class... Args>
    struct call_from_object
    {
        F(Obj::*fptr_);
        Obj *caller_;

    public:
        call_from_object(F (Obj::*fptr), Obj *caller)
            : fptr_(fptr), caller_(caller) {}
        auto operator()(Args... args)
        {
            return (caller_->*fptr_)(std::forward<Args>(args)...);
        }
    };
    
    template <class T, class... Args>
    constexpr std::unique_ptr<T> safe_make_unique_impl(Args &&...args)
    {
        return std::make_unique<T>(std::forward<Args>(args)...);
    }
    
    template <class T, class... Args>
    constexpr std::shared_ptr<T> safe_make_shared_impl(Args &&...args)
    {
        return std::make_shared<T>(std::forward<Args>(args)...);
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

    template <class Func>
    struct task_wrapped;

    template <class R, class... Args>
    struct task_wrapped<R(Args...)>
    {
        using F = R(Args...);

    public:
        const std::function<typename detail::remove_function_top_const<F>::type> taskUnwrapped_;

    public:
        template <typename Obj>
        task_wrapped(F(Obj::*fptr), Obj *caller)
            : taskUnwrapped_(detail::call_from_object<F, Obj, Args...>{fptr, caller})
        {
        }

        template <typename Obj>
        task_wrapped(F(Obj::*fptr), std::shared_ptr<Obj> caller)
            : taskUnwrapped_(detail::call_from_object<F, Obj, Args...>{fptr, &*caller})
        {
        }
        
        task_wrapped(std::function<F> &&taskUnwrapped)
            : taskUnwrapped_(std::move(taskUnwrapped))
        {
        }

        task_wrapped(std::function<F> &taskUnwrapped)
            : taskUnwrapped_(taskUnwrapped)
        {
        }

    public:
        auto operator()(Args &&...args) const noexcept
        {
            using detail::dummy;
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
                return taskUnwrapped_(std::forward<Args>(args)...), std::optional<dummy>(dummy{});
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
                          << boost::typeindex::type_id<F>() << '\n';
            }
            return decltype(taskUnwrapped_(std::forward<Args>(args)...), std::optional<dummy>())();
        }
    };

    template <class T, class... Args>
    constexpr std::unique_ptr<T> safe_make_unique(Args &&...args) noexcept
    {
        return task_wrapped<std::unique_ptr<T>(Args...)>
            (detail::safe_make_unique_impl<T, Args...>)(std::forward<Args>(args)...).value();
    }
    
    template <class T, class... Args>
    constexpr std::shared_ptr<T> safe_make_shared(Args &&...args) noexcept
    {
        return task_wrapped<std::shared_ptr<T>(Args...)>
            (detail::safe_make_shared_impl<T, Args...>)(std::forward<Args>(args)...).value();
    }
}