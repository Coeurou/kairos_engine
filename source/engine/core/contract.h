#pragma once

#include <exception>
#include <core/logger.h>
#include <core/types.h>

namespace kairos {

inline void expects(bool condition, string_view message) noexcept {
    if (!condition) {
        log_error(LoggerName::SYSTEM, "Pre-condition not satisfied: {}", message.data());
        std::quick_exit(EXIT_FAILURE);
    }
}

inline void expects(bool condition) noexcept { expects(condition, "terminate program"); }

inline void ensures(bool condition, string_view message) noexcept {
    if (!condition) {
        log_error(LoggerName::SYSTEM, "Post-condition not satisfied: {}", message.data());
        std::quick_exit(EXIT_FAILURE);
    }
}

inline void ensures(bool condition) noexcept { ensures(condition, "terminate program"); }

} // namespace kairos