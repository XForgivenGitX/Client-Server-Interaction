#pragma once

#include <vector>
#include <string>
#include <regex>
#include <optional>
#include <algorithm>
#include <boost/system/error_code.hpp>

#define COMMAND_INDEX 1
#define ARGUMENT_INDEX 2
#define INDET_VALUE 1
#define NAME_INDEX 0
#define PASS_INDEX 1

namespace common
{
    enum class limits : unsigned
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
        ECHO_LOG_IN_REQ,
        LOG_IN_RESP,
        SUCCESS_LOG_IN_RESP,

        REGISTER_REQ,
        ECHO_REGISTER_REQ,
        REGISTER_RESP,
        SUCCESS_REG_RESP,

        CREATE_ROOM_REQ,
        SUCCESS_CREATE_ROOM_RESP,
        ERROR_CREATE_ROOM_RESP,

        JOIN_ROOM_REQ,
        SUCCESS_JOIN_ROOM_RESP,
        ERROR_JOIN_ROOM_RESP,

        SEND_MESSAGE,
        DETACH_ROOM_REQ,
        DETACH_ROOM_RESP,
    };

    enum class ncommand : unsigned
    {
        REGISTER_REQ,
        LOG_IN_REQ,
        
        ERROR_REGISTER_RESP,
        ERROR_LOG_IN_RESP,

        SUCCESS_REGISTER_RESP,
        SUCCESS_LOG_IN_RESP,
        
        CREATE_ROOM_REQ,
        SUCCESS_CREATE_ROOM_RESP,
        ERROR_CREATE_ROOM_RESP,

        JOIN_ROOM_REQ,
        SUCCESS_JOIN_ROOM_RESP,
        ERROR_JOIN_ROOM_RESP,

        SEND_MESSAGE,
        DETACH_ROOM_REQ,
        DETACH_ROOM_RESP,
    };

    struct splited_frame
    {
        typedef std::vector<std::string> args_t;

    public:
        command cmd_;
        args_t args_;
    };
    typedef std::optional<common::splited_frame> splited_frame_opt_t;

    splited_frame_opt_t disassemble_frame(const std::string &frame);
    std::string assemble_frame(common::command cmd);
    std::string assemble_frame(command cmd, std::vector<std::string> args);
    splited_frame_opt_t parse_and_chek_errors(const std::string &frame, const boost::system::error_code &error);
}