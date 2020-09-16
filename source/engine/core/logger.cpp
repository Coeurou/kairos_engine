#include <logger.h>

#include <chrono>
#include <fmt/chrono.h>
#include <nameof.hpp>

std::string get_formatted_date() {
    using namespace std::chrono;
    system_clock::time_point now = system_clock::now();
    system_clock::duration fraction = now.time_since_epoch();
    fraction -= duration_cast<seconds>(fraction);

    time_t timer = system_clock::to_time_t(now);
    tm tm_info;
#ifdef __APPLE__
    localtime_r(&timer, &tm_info);
#else
    localtime_s(&tm_info, &timer);
#endif
    return fmt::format("{:%Y-%m-%d %H:%M:%S}.{:#03d}", tm_info,
                       static_cast<unsigned>(fraction / milliseconds(1)));
}

std::string format_log(LoggerName logger) {
    return fmt::format("{} | {:<10} | INFO  | ", get_formatted_date(), NAMEOF_ENUM(logger));
}