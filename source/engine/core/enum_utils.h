#pragma once

#include <type_traits>
#include <core/types.h>

template<class Enum> constexpr size_t to_index(Enum e) noexcept {
    static_assert(std::is_enum_v<Enum>, "to_index is available only for enum types.");
    return static_cast<size_t>(e);
}

template<class Enum> constexpr uint32 to_index32(Enum e) noexcept {
    static_assert(std::is_enum_v<Enum>, "to_index is available only for enum types.");
    return static_cast<uint32>(e);
}

template<class Enum> constexpr Enum to_enum(size_t index) noexcept {
    static_assert(std::is_enum_v<Enum>, "to_enum is available only for enum types.");
    static_assert(index < Enum::count, "index is greater than count.");
    return static_cast<Enum>(index);
}