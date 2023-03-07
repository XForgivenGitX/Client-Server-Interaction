#pragma once
#include <vector>
#include <string>
#include <regex>
#include <iostream>

namespace common
{
    enum class protocol : unsigned
    {
        MESSAGE_LOW_LEVEL = 1,
        MESSAGE_HIGH_LEVEL = 50,
        NAME_LOW_LEVEL = 5,
        NAME_HIGH_LEVEL = 20
    };

    enum class command : unsigned
    {
        CONNECT_REQUEST,
        SEND_REQUEST,
        AUTHORIZ_REQUEST,
        DISCONNECT_REQUEST,

        CONNECT_STATUS,
        AUTHORIZ_STATUS,
        SEND_STATUS,
        DISCONNECT_STATUS
    };

    struct splited_frame
    {
        typedef std::vector<std::string> args_t;

    public:
        command cmd;
        args_t args;

    public:
        splited_frame(std::string &&frame)
        {
            const std::regex reg("(\\[)(\\$)([0-9])(\\$)(({)[(\\w)]+(}))+(\\])");
            std::cmatch tokens;
            if (std::regex_match(frame.c_str(), tokens, reg))
                for (int i = 0; i < tokens.size(); ++i)
                    std::cout << tokens[i] << std::endl;//для отладки
        }
    };

}