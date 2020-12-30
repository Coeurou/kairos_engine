#include <input/service/input_service.h>
#include <input/service/detail/input_service_helpers.h>

namespace kairos {

/** Member functions */

result<uint32> input_service::enable(input_device device) {
    if (detail::enable(device)) {
        return result<uint32>(EXIT_SUCCESS);
    }
    else {
        return result<uint32>(error_type::runtime_error);
    }
}

void input_service::process_events(const array<system_event>& events) {
    for (const auto& e : events) {
        switch (type(e)) {
        case event_type::key_press:
        case event_type::key_release: {
            const auto& k = cast<keyboard_event>(e);
            my_keyboard.my_keys_status[to_index(k.my_key)] = k.my_action;
            break;
        }
        case event_type::controller_button_press:
        case event_type::controller_button_release: {
            const auto& c = cast<controller_event>(e);
            my_controller.my_buttons_status[to_index(c.my_button)] = c.my_action;
            break;
        }
        case event_type::controller_axis_move: {
            const auto& c = cast<controller_event>(e);
            my_controller.my_axis_status[to_index(c.my_axis)] = c.my_axis_value;
            break;
        }
        case event_type::controller_device: {
            reset(my_controller);

            const auto& c = cast<controller_event>(e);
            if (c.my_device_state != controller_device_state::removed) {
                my_controller.is_connected = true;
                my_controller.my_id = c.my_controller_id;
            }
            break;
        }
        }
    }
}

} // namespace kairos