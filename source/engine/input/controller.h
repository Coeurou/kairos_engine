#pragma once

enum class controller_device_state {
    added,
    removed,
    remapped,
    count
};

enum class controller_axis {
    left,
    right,
    trigger,
    count
};

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