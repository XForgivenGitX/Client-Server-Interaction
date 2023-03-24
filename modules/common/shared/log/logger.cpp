#include <logger.hpp>

void init_logger()
{
#ifdef ENABLE_FILE_LOGGING
    boost::log::add_file_log(
        boost::log::keywords::file_name = "sample_%N.log",
        boost::log::keywords::rotation_size = 10 * 1024 * 1024,
        boost::log::keywords::time_based_rotation = boost::log::sinks::file::rotation_at_time_point(0, 0, 0),
        boost::log::keywords::format = "[%TimeStamp%]- <%Severity%>: %Message%");

    boost::log::core::get()->set_filter(
        boost::log::trivial::severity >= boost::log::trivial::info);
#endif
    boost::log::add_console_log(std::cout, boost::log::keywords::format = "[%TimeStamp%]<%Severity%>: %Message%");
    boost::log::core::get()->set_filter(
        boost::log::trivial::severity >= boost::log::trivial::info);

    boost::log::add_common_attributes();
}