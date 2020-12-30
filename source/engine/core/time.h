#pragma once

#include <chrono>
#include <SDL.h>
#include <core/algorithms.h>
#include <core/types.h>
#include <core/logger.h>

namespace {
uint32 our_frame_number = 0;
auto our_frame_start_time = std::chrono::system_clock::now();
float our_deltatime = 0.f;
}; // namespace

namespace kairos {

struct clock {
    double delta_time = 0.0;
    double elapsed_time = 0.0;

    static double get_time() { return static_cast<double>(SDL_GetTicks()); }
};

/** the scoped object returned is [[nodiscard]], it ensures profiling to be done */
[[nodiscard]] inline auto profile_function(const char* function_name) {
    auto start_time = std::chrono::system_clock::now();
    return scoped_object(
        []() {},
        [&start_time, function_name]() {
            const auto exec_duration = std::chrono::duration_cast<std::chrono::microseconds>(
                std::chrono::system_clock::now() - start_time);
            log(LoggerName::PROFILER, "{}:{}us", function_name, exec_duration.count());
        });
}

inline uint32 frame_number() { return our_frame_number; }

inline auto current_time() { return std::chrono::system_clock::now(); }

inline float deltatime() { return our_deltatime; }

/** new frame will increment frame number and compute deltatime */
inline void new_frame() {
    our_frame_number++;
    our_deltatime = std::chrono::duration<float>(current_time() - our_frame_start_time).count();
    our_frame_start_time = current_time();
}

}; // namespace kairos