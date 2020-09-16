#pragma once

#include <exception>
#include <logger.h>
#include <types.h>

inline void expects(bool condition, string_view message) noexcept {
    if (!condition) {
        log_error(LoggerName::SYSTEM, "Pre-condition not satisfied: {}", message.data());
        std::terminate();
    }
}

inline void expects(bool condition) noexcept {
    expects(condition, "terminate program");
}

inline void ensures(bool condition, string_view message) noexcept {
    if (!condition) {
        log_error(LoggerName::SYSTEM, "Post-condition not satisfied: {}", message.data());
        std::terminate();
    }
}

inline void ensures(bool condition) noexcept {
    ensures(condition, "terminate program");
}