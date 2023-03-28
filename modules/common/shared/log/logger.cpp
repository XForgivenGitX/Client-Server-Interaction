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
  
    std::string command_to_str(common::command cmd)
    {
        using namespace common;
        switch(cmd)
        {
        case command::REGISTER:             return "REGISTER";
        case command::LOG_IN:               return "LOG_IN";
        case command::ERROR_REGISTER:       return "ERROR_REGISTER";
        case command::ERROR_LOG_IN:         return "ERROR_LOG_IN";
        case command::SUCCESS_REGISTER:     return "SUCCESS_REGISTER";
        case command::SUCCESS_LOG_IN:       return "SUCCESS_LOG_IN";
        case command::CREATE_ROOM:          return "CREATE_ROOM";
        case command::SUCCESS_CREATE_ROOM:  return "SUCCESS_CREATE_ROOM";
        case command::ERROR_CREATE_ROOM:    return "ERROR_CREATE_ROOM";
        case command::JOIN_ROOM:            return "JOIN_ROOM";
        case command::SUCCESS_JOIN_ROOM:    return "SUCCESS_JOIN_ROOM";
        case command::ERROR_JOIN_ROOM:      return "ERROR_JOIN_ROOM";
        case command::SEND_MESSAGE:         return "SEND_MESSAGE";
        case command::RECEIVE_MESSAGE:      return "RECEIVE_MESSAGE";
        case command::DETACH_ROOM:          return "DETACH_ROOM";
        default:                            return "UNKNOWN";
        }
    }

}