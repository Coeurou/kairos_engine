#pragma once

#include <core/types.h>

namespace detail {
    // FNV-1a 32bit hashing algorithm.
    constexpr uint32 fnv1a_32(const char* s, size_t count) {
        return ((count ? fnv1a_32(s, count - 1) : 2166136261u) ^ s[count]) * 16777619u;
    }
} // namespace detail

constexpr uint32 operator"" _hash(char const* s, size_t count) {
    return detail::fnv1a_32(s, count);
}

constexpr uint32 hash(string_view s) {
    return detail::fnv1a_32(s.data(), count(s));
}