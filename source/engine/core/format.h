#pragma once

#include <fmt/format.h>

namespace kairos {

template <typename S, typename... Args> auto format(const S& format_str, Args&&... args) {
    return fmt::format(format_str, std::forward<Args>(args)...);
}

} // namespace kairos