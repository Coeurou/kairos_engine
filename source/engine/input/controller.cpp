#include <input/controller.h>

#include <algorithm>

namespace kairos {

bool is_pressed(const controller& c, controller_button button) {
    const auto button_status = c.my_buttons_status[to_index(button)];
    return is_connected(c) && (button_status == action::press || button_status == action::repeat);
}

bool is_released(const controller& c, controller_button button) {
    return is_connected(c) && (c.my_buttons_status[to_index(button)] == action::release);
}

int value(const controller& c, controller_button button) { return is_pressed(c, button) ? 1 : 0; }

int value(const controller& c, controller_axis axis) {
    return is_connected(c) ? c.my_axis_status[to_index(axis)] : 0;
}

bool is_connected(const controller& c) { return c.is_connected; }

void reset(controller& c) {
    std::fill(std::begin(c.my_axis_status), std::end(c.my_axis_status), 0);
    std::fill(std::begin(c.my_buttons_status), std::end(c.my_buttons_status), action::count);
    c.is_connected = false;
    c.my_id = 0;
}

} // namespace kairos