#pragma once

#include <vector>
#include <string>
#include <regex>
#include <optional>
#include <algorithm>

namespace common
{
    namespace protocol
    {
        constexpr unsigned COMMAND_INDEX = 1;
        constexpr unsigned ARGUMENT_INDEX = 2;
        constexpr unsigned INDET_VALUE = 1;
        constexpr unsigned NAME_INDEX = 0;
        constexpr unsigned PASS_INDEX = 1;
    }

    namespace limits
    {
        constexpr unsigned MESSAGE_LOW_LENGHT = 1;
        constexpr unsigned MESSAGE_HIGH_LENGHT = 50;
        constexpr unsigned ROOM_NAME_LOW_LENGHT = 5;
        constexpr unsigned ROOM_NAME_HIGH_LENGHT = 20;
        constexpr unsigned NAME_LOW_LENGHT = 5;
        constexpr unsigned NAME_HIGH_LENGHT = 20;
    }

    enum class command : unsigned
    {
        REGISTER,
        LOG_IN,

        ERROR_REGISTER,
        ERROR_LOG_IN,

        SUCCESS_REGISTER,
        SUCCESS_LOG_IN,

        CREATE_ROOM,
        SUCCESS_CREATE_ROOM,
        ERROR_CREATE_ROOM,

        JOIN_ROOM,
        SUCCESS_JOIN_ROOM,
        ERROR_JOIN_ROOM,

        SEND_MESSAGE,
        RECEIVE_MESSAGE,

        DETACH_ROOM,
    };

    struct splited_package
    {
        typedef std::string packet_t;
        typedef std::vector<packet_t> args_t;

    protected:
        args_t args_;
        command cmd_;

    public:
        [[nodiscard]] 
        command get_command() const;
        
        [[nodiscard]] 
        packet_t get_argument(std::size_t argIndex) const;
    };

    struct transf_package : public splited_package
    {
    private:
        bool isMatchedPack = true;

    private:
        bool is_correct_argument(std::size_t argIndex);

    public:
        transf_package() = default;
        bool isMatched() const;
        void disassemble(const packet_t &);
    };

    [[nodiscard]] 
    transf_package::packet_t assemble_package(command cmd, 
                        const transf_package::args_t &args);

    [[nodiscard]] 
    transf_package::packet_t assemble_package(command cmd);
}