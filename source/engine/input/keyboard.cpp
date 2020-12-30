#include <input/keyboard.h>

namespace kairos {

bool is_pressed(const keyboard& keyboard, key key) {
    const auto key_status = keyboard.my_keys_status[to_index(key)];
    return key_status == action::press || key_status == action::repeat;
}

bool is_released(const keyboard& keyboard, key key) {
    return keyboard.my_keys_status[to_index(key)] == action::release;
}

int value(const keyboard& keyboard, key key) { return is_pressed(keyboard, key) ? 1 : 0; }

} // namespace kairos