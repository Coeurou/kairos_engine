#pragma once

#include <imgui.h>

#include <imgui_backend.h>
#include <core/types.h>

struct SDL_Window;
struct SDL_Cursor;

namespace kairos {

class imgui_sdl_backend : public imgui_backend {
public:
    bool setup(window window) override;
    void cleanup() override;
    void update() override;
    //void process_event(const variant_event& event);

    SDL_Window* my_window;
private:
    void update_gamepads();
    void update_mouse_keyboard();
    void update_cursor();

    static_array<bool, 3> my_mouse_button_pressed = { false, false, false };
    static_array<SDL_Cursor*, ImGuiMouseCursor_COUNT> my_mouse_cursors = {};
    bool can_use_mouse_global_state = true;
};

} // namespace kairos