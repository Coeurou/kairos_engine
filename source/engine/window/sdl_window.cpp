#pragma once

#include <window/sdl_window.h>

#include <core/contract.h>
#include <core/logger.h>
#include <core/messages.h>

namespace kairos {

/** Member functions */

sdl_window::sdl_window(const window_params& params) : my_native_window(nullptr) {
    expects(SDL_WasInit(SDL_INIT_VIDEO) != 0, "SDL video not initialized");
    int pos_x = (params.my_pos.x == window_params::our_undefined_pos)
                    ? SDL_WINDOWPOS_UNDEFINED
                    : static_cast<int>(params.my_pos.x);
    int pos_y = (params.my_pos.y == window_params::our_undefined_pos)
                    ? SDL_WINDOWPOS_UNDEFINED
                    : static_cast<int>(params.my_pos.y);
    my_native_window =
        SDL_CreateWindow(params.my_title.c_str(), pos_x, pos_y, static_cast<int>(params.my_size.x),
                         static_cast<int>(params.my_size.y), params.my_flags);

    ensures(my_native_window != nullptr,
            fmt::format("Couldn't create a SDL window: {}", SDL_GetError()));
}

sdl_window::sdl_window(SDL_Window* native_window) : my_native_window(native_window) {
    ensures(my_native_window != nullptr);
}

sdl_window::sdl_window(sdl_window&& other) noexcept : my_native_window(std::exchange(other.my_native_window, nullptr)) {
}

sdl_window& sdl_window::operator=(sdl_window&& other) noexcept {
    my_native_window = std::exchange(other.my_native_window, nullptr);

    return *this;
}

/** Non-member functions */

uint32 id(const sdl_window& window) {
    return static_cast<uint32>(SDL_GetWindowID(window.my_native_window));
}

vec2i size(const sdl_window& window) {
    vec2i size{0, 0};
    SDL_GetWindowSize(window.my_native_window, &size.x, &size.y);
    return size;
}

void resize(const sdl_window& window, const vec2i& size) {
    SDL_SetWindowSize(window.my_native_window, size.x, size.y);
}

void show(const sdl_window& window) { SDL_ShowWindow(window.my_native_window); }

bool is_shown(const sdl_window& window) {
    uint32 flags = SDL_GetWindowFlags(window.my_native_window);
    return (flags & SDL_WINDOW_SHOWN) > 0;
}

void hide(const sdl_window& window) { SDL_HideWindow(window.my_native_window); }

bool is_hidden(const sdl_window& window) {
    uint32 flags = SDL_GetWindowFlags(window.my_native_window);
    return (flags & SDL_WINDOW_HIDDEN) > 0;
}

void destroy(sdl_window& window) {
    SDL_DestroyWindow(window.my_native_window);
    window.my_native_window = nullptr;
}

} // namespace kairos