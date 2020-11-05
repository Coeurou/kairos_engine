#pragma once

#include <SDL.h>

#include <window/window.h>

class sdl_window : public window_interface {
public:
    sdl_window(SDL_Window* native_window);
    ~sdl_window();
    
    vec2i size() const override;
    void set_size(vec2i size) override;
    void start_frame() override;
    void end_frame() override;
    bool should_close() const override;
    void set_should_close(bool should_close) override;

    void window_resized() override;

    static std::unique_ptr<sdl_window> create(window_params params);

    SDL_Window* my_native_window = nullptr;

private:
    bool my_window_should_close = false;
};