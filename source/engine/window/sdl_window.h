#pragma once

#include <SDL.h>

#include <window/window.h>

namespace kairos {

/**
 * sdl_window is a platform independant representation of a window, powered by SDL.
 * sdl_window is NonCopyable & NonMovable, it encapsulates SDL Window through a kairos object.
 * It implements size, show, hide functions
 */
class sdl_window {
  public:
    /** Member functions */

    /** Construct a window with a size, title and redering flags. Will invoke SDL_CreateWindow.
     * Postcondition: my_native_window != nullptr */
    sdl_window(const window_params& params);

    /** Construct a window from a valid SDL_Window. Postcondition: my_native_window != nullptr */
    sdl_window(SDL_Window* native_window);

    sdl_window(const sdl_window& other) = default;
    sdl_window& operator=(const sdl_window& other) = default;
    sdl_window(sdl_window&& other) noexcept;
    sdl_window& operator=(sdl_window&& other) noexcept;

    /** access the native SDL_Window */
    SDL_Window* my_native_window = nullptr;
};

/** Non-member functions */

/** Return the id stored in SDL */
uint32 id(const sdl_window& window);

/** Return the size of a window */
vec2i size(const sdl_window& window);

/** Resize a window */
void resize(const sdl_window& window, const vec2i& size);

/** Show a window */
void show(const sdl_window& window);

/** Return the result is flags containing SDL_VISIBLE */
bool is_shown(const sdl_window& window);

/** Hide a window */
void hide(const sdl_window& window);

/** Return the result is flags containing SDL_HIDDEN */
bool is_hidden(const sdl_window& window);

/** Destroy a window */
void destroy(sdl_window& window);

} // namespace kairos