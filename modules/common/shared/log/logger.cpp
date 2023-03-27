#include "logger.hpp"
namespace lg
{
    void init_logger()
    {
#ifdef ENABLE_FILE_LOGGING
        lg::add_file_log
        (
            keywords::file_name = "sample_%N.log",
            keywords::rotation_size = 10 * 1024 * 1024,
            keywords::time_based_rotation = sinks::file::rotation_at_time_point(0, 0, 0),
            keywords::format = "[%TimeStamp%]<%Severity%>:%Message%"
        );
        lg::core::get()->set_filter(trivial::severity >= trivial::info);
#else
        lg::add_console_log(std::cout, keywords::format = "[%TimeStamp%]<%Severity%>:%Message%");
        lg::core::get()->set_filter(trivial::severity >= trivial::info);
#endif
        lg::add_common_attributes();
    }

    const char *address_cat(const char *lhs, const void *rhs)
    {
        std::ostringstream stream;
        stream << lhs << rhs;

        return stream.str().c_str();
    }

    std::string boolalpha_cast(bool rhs)
    {
        if(rhs) return "true";
        else return "false";
    }
}