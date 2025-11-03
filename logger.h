#ifndef LOGGER_H
#define LOGGER_H

#include <source_location>
#include <tuple>
#include <iostream>
#include <functional>
#include <stacktrace>

#include "logevent.h"

class Logger
{
private:
    int m_level;
    const char* m_name;
    std::function<void(LogEvent& evt)> m_outputter;

public:
    static constexpr int TRACE = 1;
    static constexpr int DEBUG = 2;
    static constexpr int INFO = 3;
    static constexpr int WARN = 4;
    static constexpr int ERROR = 5;
    static constexpr int OFF = 10;

    Logger(const char* name, std::function<void(LogEvent& evt)> outputter);

    void set_level(int level);

    template<typename... T>
    auto trace(const std::format_string<T...> fmt, T... args){
        return do_log(TRACE, fmt, args...);
    }

    template<typename... T>
    auto debug(const std::format_string<T...> fmt, T... args){
        return do_log(DEBUG, fmt, args...);
    }

    template<typename... T>
    auto info(const std::format_string<T...> fmt, T... args){
        return do_log(INFO, fmt, args...);
    }

    template<typename... T>
    auto warn(const std::format_string<T...> fmt, T... args){
        return do_log(WARN, fmt, args...);
    }

    template<typename... T>
    auto error(const std::format_string<T...> fmt, T... args){
        return do_log(ERROR, fmt, args...);
    }

private:
    template<typename... T>
    auto do_log(int level, const std::format_string<T...> fmt, T... args){
        bool do_log = m_level <= level;
        auto tup = std::make_tuple(args...);
        auto sv = fmt.get();
        return [sv2 = std::move(sv), tup = std::move(tup), do_log, level, this](const std::source_location location = std::source_location::current(),
                const std::stacktrace stacktrace = std::stacktrace::current()){
            if(!do_log){
                // do nothing
                return;
            }

            auto format_args = std::apply([](auto&&... args) {
                return std::make_format_args(args...);
            }, tup);

            LogEvent evt(sv2, format_args, location, stacktrace, level, m_name);
            if(m_outputter){
                m_outputter(evt);
            }
        };
    }
};

#endif // LOGGER_H
