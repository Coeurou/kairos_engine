#pragma once

#include <core/enum_utils.h>
#include <core/types.h>
#include <input/input.h>

namespace kairos {

enum class controller_device_state { added, removed, remapped, count };

enum class controller_axis { left_x, left_y, right_x, right_y, trigger_left, trigger_right, count };

enum class controller_button {
    button_A,
    button_B,
    button_X,
    button_Y,
    back,
    start,
    guide,
    left_stick,
    right_stick,
    left_shoulder,
    right_shoulder,
    dpad_up,
    dpad_down,
    dpad_left,
    dpad_right,
    count
};

struct controller {
    static_array<action, to_index(controller_button::count)> my_buttons_status = {action::count};
    static_array<int, to_index(controller_axis::count)> my_axis_status = {0};
    bool is_connected = false;
    uint32 my_id = 0;
};

bool is_pressed(const controller& c, controller_button button);
bool is_released(const controller& c, controller_button button);

int value(const controller& c, controller_button button);

int value(const controller& c, controller_axis axis);

bool is_connected(const controller& c);

void reset(controller& c);

} // namespace kairos