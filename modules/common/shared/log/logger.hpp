#pragma once
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/utility/setup/console.hpp>
#include "protocol.hpp"

namespace lg
{
    namespace lg = boost::log;
    namespace trivial = lg::trivial;
    namespace keywords = lg::keywords;
    namespace sinks = boost::log::sinks;

    template <typename T>
    struct add_format
    {
        const T &ref_;

    public:
        add_format(const T &ref) : ref_(ref) {}
    };

    template <typename T>
    std::ostringstream &operator<<(std::ostringstream &stream, add_format<T> value)
    {
        stream << "\n\t$" << value.ref_;
        return stream;
    }

    template <typename... Args>
    std::string build_log(const char *source, Args &&...args)
    {
        std::ostringstream stream;
        stream << "\n@" << source;
        (stream << ... << add_format(std::forward<Args>(args))) << "\n\n";
        return stream.str();
    }
    void init_logger();
    const char *address_cat(const char *lhs, const void *rhs);
    std::string boolalpha_cast(bool rhs);
    std::string command_to_str(common::command cmd);
}
