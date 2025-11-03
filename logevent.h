#ifndef LOGEVENT_H
#define LOGEVENT_H

#include <string_view>
#include <tuple>
#include <format>
#include <source_location>
#include <stacktrace>
#include <chrono>

class LogEvent
{
public:
    LogEvent(const std::string_view& format,
             const std::format_args& fmt_args,
             const std::source_location& source_loc,
             const std::stacktrace& stacktrace,
             int level,
             const std::string_view name);

    std::string render();
    std::string_view level_to_string();

private:
    std::string m_rendered_msg;

public:
    const std::string_view m_fmt;
    const std::format_args m_fmt_args;
    const std::source_location m_source_loc;
    const std::stacktrace m_stacktrace;
    const int m_level;
    const std::chrono::high_resolution_clock::time_point m_time;
    const std::string_view m_logger_name;
};

#endif // LOGEVENT_H
