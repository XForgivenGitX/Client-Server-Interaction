#include "protocol.hpp"
#include <iostream>
using namespace std::literals::string_literals;

namespace common
{
    command splited_package::get_command() const
    {
        return cmd_;
    }

    splited_package::packet_t splited_package::get_argument(std::size_t argIndex) const
    {
        return args_.at(argIndex);
    }

    void transf_package::disassemble(const packet_t &pack)
    {
        static const std::regex expr(R"(<(\d+)(\>(\<[^<>]+>)*)?)");
        static const std::regex args(R"(<([^<>]*)>)");
        std::smatch tokens;
        if (!std::regex_match(pack, tokens, expr))
        {
            isMatchedPack = false;
            return;
        }
        cmd_ = static_cast<command>(std::stoi(tokens[protocol::COMMAND_MATCH_INDEX]));
        auto arg_token = tokens[protocol::ARGUMENT_MATCH_INDEX];
        args_.resize(0);
        if (arg_token.matched)
        {
            std::copy(
                std::sregex_token_iterator(arg_token.first, arg_token.second, args, protocol::SUBMATCH_VAL),
                std::sregex_token_iterator{},
                std::back_insert_iterator<args_t>(args_));
        }
        if (!is_correct_argument(args_.size()))
            isMatchedPack = false;
        else isMatchedPack = true;
    }

    bool transf_package::is_correct_argument(std::size_t argIndex)
    {
        switch (cmd_)
        {
        case command::LOG_IN:
        case command::REGISTER:
            if (argIndex > 2)
                return false;
            break;
        case command::CREATE_ROOM:
        case command::SEND_MESSAGE:
        case command::RECEIVE_MESSAGE:
        case command::JOIN_ROOM:
            if (argIndex > 1)
                return false;
            break;
        
        default:
            if (argIndex != 0)
                return false;
        }
        return true;
    }

    bool transf_package::isMatched() const
    {
        return isMatchedPack;
    }

    transf_package::packet_t assemble_package(command cmd)
    {
        return "<"s + std::to_string(static_cast<unsigned>(cmd)) + ">"s;
    }

    transf_package::packet_t assemble_package(command cmd, const transf_package::args_t &args)
    {
        transf_package::packet_t pack = assemble_package(cmd);
        for (auto &arg : args)
            if(!arg.empty())
                pack += "<"s + arg + ">"s;
        return pack;
    }
}