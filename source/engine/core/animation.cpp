#include <core/animation.h>
#include <core/contract.h>
#include <core/easing_lib.h>

void variant_animation::start_impl() {
    expects(my_value != nullptr, "Nothing to animate");
}

void variant_animation::update_impl(float dt) {
    expects(my_value != nullptr, "Nothing to animate");

    float abscissa = 0.f;
    if (const auto duration_in_seconds = std::get_if<float>(&my_duration)) {
        abscissa = (std::get<float>(my_current_timestamp) / *duration_in_seconds);
    }
    else if (const auto duration_in_frames = std::get_if<int>(&my_duration)) {
        abscissa = (std::get<int>(my_current_timestamp) / static_cast<float>(*duration_in_frames));
    }
    float t = my_easing_curve.evaluate(abscissa);
    /*std::visit(
        [&](auto&& val) {
            using T = std::decay_t<decltype(val)>;
            val = lerp(std::get<T>(my_start_value), std::get<T>(my_end_value), t);
        }, *my_value);*/
}

void variant_animation::pause_impl() {
}

void variant_animation::resume_impl() {
    expects(my_value != nullptr, "Nothing to animate");
}

void variant_animation::stop_impl() {
}