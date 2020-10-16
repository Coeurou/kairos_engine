#pragma once

#include <sdl_application.h>
#include <chrono>
#include <random>

#include <imgui.h>

#include <contract.h>

bool sdl_application::init() {
    my_type = implementation_type::SDL;
    expects(SDL_Init(SDL_INIT_VIDEO) >= 0, fmt::format("Couldn't initilize SDL: {}", SDL_GetError()));
    return true;
}

void sdl_application::exec() {
    frame_number++;
}

void sdl_application::exit() {
    SDL_Quit();
    my_gl_context.reset(nullptr);
}

array<variant_event> sdl_application::process_events() {
    static constexpr int event_queue_empty = 0;
    SDL_Event e;
    array<variant_event> polled_events{};
    polled_events.reserve(4);
    while (SDL_PollEvent(&e) != event_queue_empty) {
        switch (e.type) {
        case SDL_QUIT: {
            my_main_window->set_should_close(true);
            break;
        }
        case SDL_MOUSEBUTTONDOWN: {
            break;
        }
        case SDL_MOUSEBUTTONUP: {
            break;
        }
        case SDL_KEYDOWN: {
            my_main_window->set_should_close(e.key.keysym.scancode == SDL_SCANCODE_ESCAPE);
            break;
        }
        case SDL_KEYUP: {
            break;
        }
        case SDL_WINDOWEVENT_RESIZED: {
            break;
        }
        }
        polled_events.emplace_back(e);
    }
    return polled_events;
}
