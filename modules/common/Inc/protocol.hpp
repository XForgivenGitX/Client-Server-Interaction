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
        ROOM_NAME_LOW_LEVEL = 5,
        ROOM_NAME_HIGH_LEVEL = 20,
        NAME_LOW_LEVEL = 5,
        NAME_HIGH_LEVEL = 20,
        COMMAND_INDEX = 1,
        ARGUMENT_INDEX = 2
        //...
    };
    //<5>%<Oleg><1234567890>

    struct splited_frame
    {
        typedef std::vector<std::string> args_t;

    public:
        int cmd_;
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