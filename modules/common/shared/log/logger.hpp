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
    using std::string;
    using std::ostringstream;
    
    namespace lg = boost::log;
    namespace trivial = lg::trivial;
    namespace keywords = lg::keywords;
    namespace sinks = boost::log::sinks;

    // template <typename T>
    // struct add_format
    // {
    //     const T &ref_;

    // public:
    //     add_format(const T &ref) : ref_(ref) {}
    // };

    // template <typename T>
    // ostringstream &operator<<(ostringstream &stream, add_format<T> value)
    // {
    //     stream << "\n\t\t$" << value.ref_;
    //     return stream;
    // }

    // template <typename... Args>
    // string build_log(const string& source, Args &&...args)
    // {
    //     ostringstream stream;
    //     stream << "\n\t@" << source;
    //     (stream << ... << add_format(std::forward<Args>(args))) << "\n";
    //     return stream.str();
    // }

    void init_logger();
    string boolalpha_cast(bool rhs);
    string build_arg(const string& arg);
    string build_src(const string& source);
    string command_to_str(common::command cmd);
}
