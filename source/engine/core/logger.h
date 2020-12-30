#pragma once

#include <fmt/format.h>
#include <string>
#include <string_view>
#include <utility>

enum class LoggerName {
    AI = 0,
    EDITOR,
    ENGINE,
    GRAPHICS,
    MESSAGE,
    PROFILER,
    SYSTEM,
    MISC
};

std::string get_formatted_date();

std::string format_log(LoggerName logger);
#ifndef NDEBUG
template <typename... Args> 
inline void log(LoggerName logger, std::string_view format_str, Args&&... args) {
    fmt::print(format_log(logger) + format_str.data(), std::forward<Args>(args)...);
}

template <typename... Args> 
inline void log_error(LoggerName logger, std::string_view format_str, Args&&... args) {
    fmt::print(stderr, format_log(logger) + format_str.data(), std::forward<Args>(args)...);
}
#else
template <typename... Args>
inline void log(LoggerName, std::string_view, Args&&...) {}

template <typename... Args>
inline void log_error(LoggerName, std::string_view, Args&&...) {}
#endif