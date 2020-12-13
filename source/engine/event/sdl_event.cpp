#include <event/sdl_event.h>

#include <SDL.h>

namespace kairos {

void peek_events(const sdl_event_tag& /*sdl*/, array<system_event>& events) {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        events.emplace_back(convert(e));
    }
}

system_event convert(const SDL_Event& e) {
    system_event res;

    switch (e.type) {
    case SDL_AUDIODEVICEADDED:
    {
        break;
    }
    case SDL_AUDIODEVICEREMOVED:
    {
        break;
    }
    case SDL_CONTROLLERAXISMOTION:
    {
        break;
    }
    case SDL_CONTROLLERBUTTONDOWN:
    {
        break;
    }
    case SDL_CONTROLLERBUTTONUP:
    {
        break;
    }
    case SDL_CONTROLLERDEVICEADDED:
    {
        break;
    }
    case SDL_CONTROLLERDEVICEREMOVED:
    {
        break;
    }
    case SDL_CONTROLLERDEVICEREMAPPED:
    {
        break;
    }
    case SDL_KEYDOWN:
    {
        break;
    }
    case SDL_KEYUP:
    {
        break;
    }
    case SDL_MOUSEMOTION:
    {
        break;
    }
    case SDL_MOUSEBUTTONDOWN:
    {
        break;
    }
    case SDL_MOUSEBUTTONUP:
    {
        break;
    }
    case SDL_MOUSEWHEEL:
    {
        break;
    }
    case SDL_QUIT:
    {
        break;
    }
    case SDL_SYSWMEVENT:
    {
        break;
    }
    case SDL_WINDOWEVENT:
    {
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
        res = system_event{ event_type::window, e.window.timestamp };
        res.my_internal = window_event{ e.window.windowID, event_type, {e.window.data1, e.window.data2} };
        break;
    }
    }
    return res;
}

} // namespace kairos
