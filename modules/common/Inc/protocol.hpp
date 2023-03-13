#pragma once

#include <vector>
#include <string>
#include <regex>
#include <optional>
#include <algorithm>

namespace common
{
    enum class protocol : unsigned
    {
        MESSAGE_LOW_LEVEL = 1,
        MESSAGE_HIGH_LEVEL = 50,
        NAME_LOW_LEVEL = 5,
        NAME_HIGH_LEVEL = 20,
        COMMAND_INDEX = 1,
        ARGUMENT_INDEX = 2
        //...
    };

    enum class command : unsigned
    {
        REPEAT_REQUEST,
        CONNECT_REQUEST,
        SEND_REQUEST,
        AUTHORIZ_REQUEST,
        DISCONNECT_REQUEST,

        REPEAT_STATUS,
        CONNECT_STATUS,
        SEND_STATUS,
        AUTHORIZ_STATUS,
        DISCONNECT_STATUS
    };

    struct splited_frame
    {
        typedef std::vector<std::string> args_t;

    public:
        command cmd_;
        args_t args_;
    };
    
    std::optional<splited_frame> split_frame(const std::string &frame);
}


/*
struct bad_server : std::exception
{
    virtual const char *what() const noexcept
    {
        return "server crashed on the initiative of the developer";
    }
    virtual ~bad_server() = default;
};

template <typename T>
struct singleton
{
    static T &get_instanse()
    {
        static T instance;
        return instance;
    }

protected:
    singleton() = default;
};
*/