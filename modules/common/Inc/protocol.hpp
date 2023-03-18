#pragma once

#include <vector>
#include <string>
#include <regex>
#include <optional>
#include <algorithm>

#define COMMAND_INDEX       1
#define ARGUMENT_INDEX      2
#define NAME_INDEX          0
#define PASS_INDEX          1

namespace common
{
    enum class protocol : unsigned
    {
        MESSAGE_LOW_LENGHT = 1,
        MESSAGE_HIGH_LENGHT = 50,
        ROOM_NAME_LOW_LENGHT = 5,
        ROOM_NAME_HIGH_LENGHT = 20,
        NAME_LOW_LENGHT = 5,
        NAME_HIGH_LENGHT = 20
    };
    enum class command : unsigned
    {
        LOG_IN_REQ,
        REPEAT_LOG_IN_REQ,
        LOG_IN_RESP,
        
        REGISTER_REQ,
        REPEAT_REG_REQ,
        REGISTER_RESP
    };
    
    struct splited_frame
    {
        typedef std::vector<std::string> args_t;

    public:
        command cmd_;
        args_t args_;
    };
    
    std::optional<splited_frame> split_frame(const std::string &frame);
    std::string assemble_frame(common::command cmd);
    std::string assemble_frame(command cmd, std::vector<std::string> args);
}