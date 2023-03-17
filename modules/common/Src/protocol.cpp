#include <protocol.hpp>

std::optional<splited_frame> common::split_frame(const std::string &frame)
{
    static const std::regex expr(R"(\[\<(\d+)(\>%(\<[^<>]+\>)*)?\])");
    static const std::regex args(R"(\<[^<>]+\>)");
    std::smatch tokens;
    splited_frame sframe;
    if (std::regex_match(frame, tokens, expr))
    {
        auto cmd = std::stoi(tokens[static_cast<int>(protocol::COMMAND_INDEX)]);
        auto arg_token = tokens[static_cast<int>(protocol::ARGUMENT_INDEX)];
        sframe.cmd_ = static_cast<command>(cmd);
        if (arg_token.matched)
            std::copy(
                std::sregex_token_iterator(arg_token.first, arg_token.second, args, 1),
                std::sregex_token_iterator{},
                std::back_insert_iterator<std::vector<std::string>>(sframe.args_));
        return sframe;
    }
    else return std::nullopt;
}