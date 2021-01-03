#pragma once

#include <algorithm>
#include <optional>
#include <core/types.h>

namespace kairos {

enum class animation_state { idle, started, paused, stopped, count };

template <class Property> class animation {
  public:
    animation(std::initializer_list<Property> samples, float duration = 1.f,
              float playback_rate = 1.f, bool repeat = false)
        : my_samples(samples), should_repeat(repeat), my_duration(duration),
          my_playback_rate(playback_rate) {}
    animation(const array<Property>& samples, float duration = 1.f, float playback_rate = 1.f,
              bool repeat = false)
        : my_samples(samples), should_repeat(repeat), my_duration(duration),
          my_playback_rate(playback_rate) {}

    void start() { my_state = animation_state::started; }

    std::optional<Property> update(float dt) {
        std::optional<Property> res = std::nullopt;

        switch (my_state) {
        case animation_state::started: {
            if (my_current_timestamp >= my_duration) {
                stop();
            }
            else {
                my_current_timestamp += dt * my_playback_rate;
                size_t sample_index =
                    static_cast<size_t>(my_current_timestamp / (my_duration / count(my_samples)));
                sample_index = std::clamp(sample_index, size_t(0), count(my_samples) - 1);
                res = my_samples[sample_index];
            }
            break;
        }
        case animation_state::paused: {
            pause();
            break;
        }
        case animation_state::stopped: {
            if (should_repeat) {
                start();
            }
            break;
        }
        case animation_state::idle:
        default:
            break;
        }
        return res;
    }

    void pause() { my_state = animation_state::paused; }

    void resume() { my_state = animation_state::started; }

    void stop() {
        my_state = animation_state::stopped;
        my_current_timestamp = 0;
    }

    animation_state get_state() const { return my_state; }

  private:
    /** Animation duration in seconds. */
    float my_duration{1.f};
    float my_playback_rate{1.f};
    float my_current_timestamp{0.f};
    animation_state my_state{animation_state::idle};
    bool should_repeat{false};
    array<Property> my_samples;
};

/** Non-member functions */

} // namespace kairos