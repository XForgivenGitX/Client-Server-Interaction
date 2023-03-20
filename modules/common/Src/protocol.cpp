#include <protocol.hpp>
using namespace std::literals::string_literals;

common::splited_frame_opt_t common::disassemble_frame(const std::string &frame)
{
    static const std::regex expr(R"(<(\d+)(\>(\<[^<>]+>)*)?)");
    static const std::regex args(R"(<([^<>]*)>)");
    std::smatch tokens;
    splited_frame sframe;
    if (std::regex_match(frame, tokens, expr))
    {
        sframe.cmd_ = static_cast<command>(std::stoi(tokens[COMMAND_INDEX]));
        auto arg_token = tokens[ARGUMENT_INDEX];
        if (arg_token.matched)
            std::copy(
                std::sregex_token_iterator(arg_token.first, arg_token.second, args, INDET_VALUE),
                std::sregex_token_iterator{},
                std::back_insert_iterator<std::vector<std::string>>(sframe.args_));
        return sframe;
    }
    else
        return std::nullopt;
}

std::string common::assemble_frame(common::command cmd)
{
    return "<"s + std::to_string(static_cast<unsigned>(cmd)) + ">"s;
}

std::string common::assemble_frame(common::command cmd, std::vector<std::string> args)
{
    std::string frame = assemble_frame(cmd);
    for (auto &arg : args)
        frame += "<"s + arg + ">"s;
    return frame;
}

common::splited_frame_opt_t common::parse_and_chek_errors(const std::string &frame, const boost::system::error_code &error)
{
    if (error)
    {
        return std::nullopt;
    }
    auto msg = common::disassemble_frame(frame);
    if (!msg)
    {
        return std::nullopt;
    }
    return msg;
}