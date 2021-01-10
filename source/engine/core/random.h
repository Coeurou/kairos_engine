#pragma once

#include <random>
#include <type_traits>
#include <core/types.h>

namespace kairos {

namespace {
    inline std::random_device rd;
    inline std::mt19937 generator(rd());
}

template<class T> T random_int(T min, T max) {
    static_assert(std::is_integral_v<T>, "Cannot get a random value for a non integral parameter");
    auto dist = std::uniform_int_distribution<T>(min, max);
    return dist(generator);
}

template<class T> T random_real(T min, T max) {
    static_assert(std::is_floating_point_v<T>, "Cannot get a random value for a non floating parameter");
    auto dist = std::uniform_real_distribution<T>(min, max);
    return dist(generator);
}

} // namespace kairos