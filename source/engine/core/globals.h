#pragma once

#include <core/types.h>

namespace globals {
    constexpr float pi = 3.14159265358979323846f;

    inline sizef pixel_size = { 1.f, 1.f };
#ifdef NDEBUG
    static const string assets_folder = R"(assets/)";
#else
#ifdef __APPLE__
    static const string assets_folder = R"(../../../assets/)";
#else
    static const string assets_folder = R"(../../assets/)";
#endif
#endif
} // namespace globals
