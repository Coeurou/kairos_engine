#pragma once

#include <window/sdl_window.h>

#include <core/contract.h>
#include <application/core_application.h>
#include <core/logger.h>
#include <core/messages.h>
#include <graphics/opengl/opengl_context.h>

sdl_window::sdl_window(SDL_Window* native_window) : my_native_window(native_window) {
}

sdl_window::~sdl_window() {
    expects(core_application::is_initialized(implementation_type::SDL), "Cannot destroy a SDL window when no SDL context is initialized.");

    SDL_DestroyWindow(my_native_window);
}

vec2i sdl_window::size() const {
    vec2i size{ 0, 0 };
    SDL_GetWindowSize(my_native_window, &size.x, &size.y);
    return size;
}

void sdl_window::set_size(vec2i size) {
    SDL_SetWindowSize(my_native_window, size.x, size.y);
    send_message(string("sdl_window"), no_recipient, MessageType::window_resized, { size });
}

void sdl_window::start_frame() {
    glClearColor(0.241f, 0.854f, 0.396f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void sdl_window::end_frame() {
    if (core_application::graphics_implementation() == renderer_engine::opengl ||
        core_application::graphics_implementation() == renderer_engine::opengl_es) {
        core_application::gl_context()->swap_buffers();
    }
}

bool sdl_window::should_close() const {
    return my_window_should_close;
}

void sdl_window::set_should_close(bool should_close) {
    my_window_should_close = should_close;
}

void sdl_window::window_resized()
{
    send_message(string("sdl_window"), no_recipient, MessageType::window_resized, { size() });
}

std::unique_ptr<sdl_window> sdl_window::create(window_params params) {
    expects(core_application::is_initialized(implementation_type::SDL));
    int pos_x = (params.my_pos.x == window_params::our_undefined_pos) ? SDL_WINDOWPOS_UNDEFINED : static_cast<int>(params.my_pos.x);
    int pos_y = (params.my_pos.y == window_params::our_undefined_pos) ? SDL_WINDOWPOS_UNDEFINED : static_cast<int>(params.my_pos.y);
    uint32 renderer_flag = SDL_WINDOW_OPENGL;
    SDL_Window* window = SDL_CreateWindow(params.my_title.c_str(), pos_x, pos_y,
        static_cast<int>(params.my_size.x), static_cast<int>(params.my_size.y), SDL_WINDOW_SHOWN /*| SDL_WINDOW_BORDERLESS*/ | renderer_flag);

    ensures(window != nullptr, fmt::format("Couldn't create a SDL window: {}", SDL_GetError()));
    log(LoggerName::SYSTEM, fmt::format("A SDL window has been successfully created: {}\n", to_string(params)));

    return std::make_unique<sdl_window>(window);
}