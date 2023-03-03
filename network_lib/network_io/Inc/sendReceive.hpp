#pragma once
#include <ClientServer.hpp>

// TODO
// 3.io__::buffer accepts a non-const argument
// 4.add debug block instead of assert
// 9.add compile-time condition on the correctness of user function arguments

namespace sendReceive
{
    struct Action
    {
    public:
        static void send(SocketDataPtr&& socketData, callback_func_t&& func);
        static void receive(SocketDataPtr&& socketData, std::size_t at_least_bytes, callback_func_t&& func);
    
    private:   
        Action() = delete;
    };

    struct CallbackFunctionWrapper
    {
    public:
        explicit CallbackFunctionWrapper(SocketDataPtr &&socketData, callback_func_t &&func);
        void operator()(const boost::system::error_code &error, std::size_t bytes_transferred) noexcept;

    private:
        SocketDataPtr socketData_;
        callback_func_t functionWrapped_;
    };
   
}