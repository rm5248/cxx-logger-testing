#include "logevent.h"
#include "logger.h"

LogEvent::LogEvent(const std::string_view& format,
                   const std::format_args& fmt_args,
                   const std::source_location& source_loc,
                   const std::stacktrace& stacktrace,
                   int level,
                   const std::string_view name) :
    m_fmt(format),
    m_fmt_args(fmt_args),
    m_source_loc(source_loc),
    m_stacktrace(stacktrace),
    m_level(level),
    m_time(std::chrono::high_resolution_clock::now()),
    m_logger_name(name)
{

}

std::string LogEvent::render(){
    if(!m_rendered_msg.empty()){
        return m_rendered_msg;
    }

    m_rendered_msg = std::vformat(m_fmt, m_fmt_args);

    return m_rendered_msg;
}

std::string_view LogEvent::level_to_string(){
    switch(m_level){
    case Logger::DEBUG:
        return "DEBUG";
    case Logger::INFO:
        return "INFO";
    }

    return "UNKN";
}
