#include <event/sdl_event.h>

#include <SDL.h>
#include <nameof.hpp>
#include <core/logger.h>
#include <input/sdl_input_backend.h>

namespace kairos {

void peek_events(const sdl_event_tag& /*sdl*/, array<system_event>& events) {
    events.clear();
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        events.emplace_back(convert(e));
    }
}

system_event convert(const SDL_Event& e) {
    system_event res;

    switch (e.type) {
    case SDL_AUDIODEVICEADDED: {
        break;
    }
    case SDL_AUDIODEVICEREMOVED: {
        break;
    }
    case SDL_CONTROLLERAXISMOTION: {
        break;
    }
    case SDL_CONTROLLERBUTTONDOWN:
    case SDL_CONTROLLERBUTTONUP: {
        bool is_button_down = (e.cbutton.state == SDL_PRESSED);
        res = system_event{is_button_down ? event_type::controller_button_press
                                          : event_type::controller_button_release,
                           e.cbutton.timestamp};
        res.my_internal = controller_event{
            static_cast<uint32>(e.cbutton.which), is_button_down ? action::press : action::release,
            our_controller_button_adapter.at(static_cast<SDL_GameControllerButton>(e.cbutton.button))};
        log(LoggerName::MISC, "SDL controller button event: {} {}\n", is_button_down ? "down" : "up",
            NAMEOF_ENUM(our_controller_button_adapter.at(
                static_cast<SDL_GameControllerButton>(e.cbutton.button))));
        break;
    }
    case SDL_CONTROLLERDEVICEADDED:
    case SDL_CONTROLLERDEVICEREMOVED:
    case SDL_CONTROLLERDEVICEREMAPPED: {
        res = system_event{event_type::controller_device, e.cbutton.timestamp};
        auto c = controller_event{static_cast<uint32>(e.cdevice.which)};
        c.my_device_state = our_controller_state_adapter.at(e.cdevice.type);
        res.my_internal = c;
        log(LoggerName::MISC, "SDL controller device event: {}\n", NAMEOF_ENUM(c.my_device_state));
        break;
    }
    case SDL_KEYDOWN: {
        res = system_event{event_type::key_press, e.key.timestamp};
        res.my_internal =
            keyboard_event{e.key.windowID, (e.key.repeat > 0) ? action::repeat : action::press,
                           our_keyboard_adapter.at(e.key.keysym.scancode)};
        log(LoggerName::MISC, "SDL key down event: {}\n",
            NAMEOF_ENUM(our_keyboard_adapter.at(e.key.keysym.scancode)));
        break;
    }
    case SDL_KEYUP: {
        res = system_event{event_type::key_release, e.key.timestamp};
        res.my_internal =
            keyboard_event{e.key.windowID, (e.key.repeat > 0) ? action::repeat : action::release,
                           our_keyboard_adapter.at(e.key.keysym.scancode)};
        log(LoggerName::MISC, "SDL key up event: {}\n",
            NAMEOF_ENUM(our_keyboard_adapter.at(e.key.keysym.scancode)));
        break;
    }
    case SDL_MOUSEMOTION: {
        break;
    }
    case SDL_MOUSEBUTTONDOWN: {
        break;
    }
    case SDL_MOUSEBUTTONUP: {
        break;
    }
    case SDL_MOUSEWHEEL: {
        break;
    }
    case SDL_TEXTINPUT: {
        res = system_event{event_type::text_input, e.text.timestamp};
        res.my_internal = text_input_event{e.text.windowID, e.text.text};
        break;
    }
    case SDL_QUIT: {
        break;
    }
    case SDL_SYSWMEVENT: {
        break;
    }
    case SDL_WINDOWEVENT: {
        window_event_type event_type = window_event_type::count;
        switch (e.window.event) {
        case SDL_WINDOWEVENT_SHOWN:
            event_type = window_event_type::show;
            break;
        case SDL_WINDOWEVENT_HIDDEN:
            event_type = window_event_type::hide;
            break;
        case SDL_WINDOWEVENT_MOVED:
            event_type = window_event_type::move;
            break;
        case SDL_WINDOWEVENT_RESIZED:
            event_type = window_event_type::resize;
            break;
        case SDL_WINDOWEVENT_MINIMIZED:
            event_type = window_event_type::minimize;
            break;
        case SDL_WINDOWEVENT_MAXIMIZED:
            event_type = window_event_type::maximize;
            break;
        case SDL_WINDOWEVENT_RESTORED:
            event_type = window_event_type::restore;
            break;
        case SDL_WINDOWEVENT_ENTER:
            event_type = window_event_type::mouse_enter;
            break;
        case SDL_WINDOWEVENT_LEAVE:
            event_type = window_event_type::mouse_leave;
            break;
        case SDL_WINDOWEVENT_FOCUS_GAINED:
            event_type = window_event_type::keyboard_gain_focus;
            break;
        case SDL_WINDOWEVENT_FOCUS_LOST:
            event_type = window_event_type::keyboard_lose_focus;
            break;
        case SDL_WINDOWEVENT_CLOSE:
            event_type = window_event_type::close;
            break;
        }
        res = system_event{event_type::window, e.window.timestamp};
        res.my_internal =
            window_event{e.window.windowID, event_type, {e.window.data1, e.window.data2}};
        break;
    }
    case SDL_DISPLAYEVENT: {
        log(LoggerName::MISC, "SDL display event is not handled, maybe it should ?\n");
        break;
    }
    case SDL_TEXTEDITING: {
        log(LoggerName::MISC, "SDL text editing event is not handled, maybe it should ?\n");
        break;
    }
    case SDL_KEYMAPCHANGED: {
        log(LoggerName::MISC, "SDL key map changed event is not handled, maybe it should ?\n");
        break;
    }
    case SDL_JOYAXISMOTION: {
        log(LoggerName::MISC, "SDL joystick axis motion event is not handled, maybe it should ?\n");
        break;
    }
    case SDL_JOYBALLMOTION: {
        log(LoggerName::MISC, "SDL joystick ball motion event is not handled, maybe it should ?\n");
        break;
    }
    case SDL_JOYHATMOTION: {
        log(LoggerName::MISC, "SDL joystick hat motion event is not handled, maybe it should ?\n");
        break;
    }
    case SDL_JOYBUTTONDOWN: {
        log(LoggerName::MISC, "SDL joystick button down event is not handled, maybe it should ?\n");
        break;
    }
    case SDL_JOYBUTTONUP: {
        log(LoggerName::MISC, "SDL joystick button up event is not handled, maybe it should ?\n");
        break;
    }
    case SDL_JOYDEVICEADDED: {
        log(LoggerName::MISC,
            "SDL joystick device added event is not handled, maybe it should ?\n");
        break;
    }
    case SDL_JOYDEVICEREMOVED: {
        log(LoggerName::MISC,
            "SDL joystick device removed event is not handled, maybe it should ?\n");
        break;
    }
    case SDL_FINGERDOWN: {
        log(LoggerName::MISC, "SDL finger down event is not handled, maybe it should ?\n");
        break;
    }
    case SDL_FINGERUP: {
        log(LoggerName::MISC, "SDL finger up event is not handled, maybe it should ?\n");
        break;
    }
    case SDL_FINGERMOTION: {
        log(LoggerName::MISC, "SDL finger motion event is not handled, maybe it should ?\n");
        break;
    }
    case SDL_DOLLARGESTURE: {
        log(LoggerName::MISC, "SDL dollar gesture event is not handled, maybe it should ?\n");
        break;
    }
    case SDL_DOLLARRECORD: {
        log(LoggerName::MISC, "SDL dollar record event is not handled, maybe it should ?\n");
        break;
    }
    case SDL_MULTIGESTURE: {
        log(LoggerName::MISC, "SDL multi gesture event is not handled, maybe it should ?\n");
        break;
    }
    case SDL_CLIPBOARDUPDATE: {
        log(LoggerName::MISC, "SDL clipboard update event is not handled, maybe it should ?\n");
        break;
    }
    case SDL_DROPFILE: {
        log(LoggerName::MISC, "SDL drop file event is not handled, maybe it should ?\n");
        break;
    }
    case SDL_DROPTEXT: {
        log(LoggerName::MISC, "SDL drop text event is not handled, maybe it should ?\n");
        break;
    }
    case SDL_DROPBEGIN: {
        log(LoggerName::MISC, "SDL drop begin event is not handled, maybe it should ?\n");
        break;
    }
    case SDL_DROPCOMPLETE: {
        log(LoggerName::MISC, "SDL drop complete event is not handled, maybe it should ?\n");
        break;
    }
    case SDL_SENSORUPDATE: {
        log(LoggerName::MISC, "SDL sensor update event is not handled, maybe it should ?\n");
        break;
    }
    case SDL_RENDER_TARGETS_RESET: {
        log(LoggerName::MISC, "SDL render targets reset event is not handled, maybe it should ?\n");
        break;
    }
    case SDL_RENDER_DEVICE_RESET: {
        log(LoggerName::MISC, "SDL render device reset event is not handled, maybe it should ?\n");
        break;
    }
    }
    return res;
}

} // namespace kairos
