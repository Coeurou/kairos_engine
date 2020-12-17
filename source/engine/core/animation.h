#pragma once

#include <core/types.h>
#include <core/easing_curve.h>
#include <graphics/sprite.h>

namespace kairos {

enum class animation_direction { forward, backward, count };

enum class animation_state { idle, started, paused, stopped, count };

template <class Property> class animation {
  public:
    animation_direction my_direction{animation_direction::forward};
    std::variant<int, float> my_duration{0};
    bool should_repeat{false};

    virtual ~animation() {}

    void start() {
        my_state = animation_state::started;
        start_impl();
    }

    void update(float dt) {
        float sign_direction = (my_direction == animation_direction::forward) ? 1 : -1.f;
        switch (my_state) {
        case animation_state::started: {
            if (const auto duration_in_seconds = std::get_if<float>(&my_duration)) {
                if (my_current_timestamp >= duration_in_seconds) {
                    stop();
                }
                my_current_timestamp += dt * sign_direction;
            } else if (const auto duration_in_frames = std::get_if<int>(&my_duration)) {
                if (my_current_timestamp >= duration_in_frames || my_current_timestamp < 0) {
                    stop();
                }
                my_current_timestamp += sign_direction;
            }
            update_impl(dt);

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
    }

    void pause() {
        my_state = animation_state::paused;
        pause_impl();
    }

    void resume() {
        my_state = animation_state::started;
        resume_impl();
    }

    void stop() {
        my_state = animation_state::stopped;
        my_current_timestamp = 0;
        stop_impl();
    }

    animation_state get_state() const { return my_state; }

  protected:
    std::variant<int, float> my_current_timestamp{0};

  private:
    animation_state my_state{animation_state::idle};

    virtual void start_impl() = 0;
    virtual void update_impl(float /*dt*/) = 0;
    virtual void pause_impl() = 0;
    virtual void resume_impl() = 0;
    virtual void stop_impl() = 0;
};

class variant_animation : public animation<variant> {
  private:
    variant my_start_value;
    variant* my_value{nullptr};
    variant my_end_value;
    easing_curve my_easing_curve{ease_type::ease_in_sine};

    void start_impl() override;
    void update_impl(float dt) override;
    void pause_impl() override;
    void resume_impl() override;
    void stop_impl() override;
};

class sprite_animation : public animation<sprite> {
  private:
    vec2i my_start_value;
    sprite* my_value{nullptr};
    vec2i my_end_value;
    easing_curve my_easing_curve{ease_type::ease_in_sine};

    void start_impl() override;
    void update_impl(float dt) override;
    void pause_impl() override;
    void resume_impl() override;
    void stop_impl() override;
};

} // namespace kairos