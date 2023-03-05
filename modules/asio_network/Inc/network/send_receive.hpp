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
        typedef std::function<void(socket_data_ptr&&, const boost::system::error_code &)> callback_func_t;
    
    public:
        static void send(socket_data_ptr &&socketData, callback_func_t &&handler);
        static void receive(socket_data_ptr &&socketData, std::size_t atLeastBytes, callback_func_t &&handler);

    private:
        struct callback_function_wrapper
        {
        private:
            socket_data_ptr socketData_;
            callback_func_t functionWrapped_;
        
        public:
            explicit callback_function_wrapper(socket_data_ptr &&socketData, callback_func_t &&handler);
            void operator()(const boost::system::error_code &error, std::size_t bytesTransferred) noexcept;
        };

    private:
        send_receive() = delete;
    };

}