#pragma once
#include <network_module.hpp>

// TODO
// 3.io__::buffer accepts a non-const argument
// 4.add debug block instead of assert
// 9.add compile-time condition on the correctness of user function arguments

namespace anet
{
    struct send_receive
    {
        typedef utility::task_wrapped<void(socket_data_ptr, 
                                const boost::system::error_code &)> callback_func_t;

    public:
        static void send(socket_data_ptr socketData, callback_func_t &&handler);
        static void receive(socket_data_ptr socketData, callback_func_t &&handler,
                            std::size_t atMostBytes = FRAME_MAX_LENGHT, 
                            std::size_t atLeastBytes = FRAME_MIN_LENGHT);

    private:
        struct callback_function_wrapper;

    private:
        send_receive() = delete;
    };
    
    struct send_receive::callback_function_wrapper
    {
    private:
        socket_data_ptr socketData_;
        callback_func_t functionWrapped_;
        tags tag_;
    
    public:
        explicit callback_function_wrapper(socket_data_ptr socketData, 
                                        callback_func_t &&handler, tags tag);
        void operator()(const boost::system::error_code &error, 
                                        std::size_t bytesTransferred) noexcept;
    };
}