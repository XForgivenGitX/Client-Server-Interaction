#pragma once
#include <vector>
#include <string>

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
        CONNECT_STATUS,
        AUTHORIZ_REQUEST,
        AUTHORIZ_STATUS,
        SEND_MESSAGE,
        SEND_STATUS,
        DISCONNECT_REQUEST,
        DISCONNECT_STATUS
    };
    
    struct splited_frame
    {
        command cmd;
        std::vector<std::string> args;
    };
    
    splited_frame split_frame(std::string&& frame)
    {

    }
}