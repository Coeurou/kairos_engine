#pragma once

#include <imgui.h>

#include <core/types.h>
#include <window/window.h>

struct SDL_Window;
struct SDL_Cursor;

namespace kairos {

struct system_event;

class imgui_sdl_backend {
  public:
    bool setup(window window);
    void cleanup();
    void update();
    void process_event(const system_event& event);

    bool is_visible() const;
    void set_visible(bool visibility);

    SDL_Window* my_window;

  private:
    void update_gamepads();
    void update_mouse_keyboard();
    void update_cursor();

    static_array<bool, 3> my_mouse_button_pressed = {false, false, false};
    static_array<SDL_Cursor*, ImGuiMouseCursor_COUNT> my_mouse_cursors = {};
    bool can_use_mouse_global_state = true;
    bool is_imgui_visible = true;
};

void show(imgui_sdl_backend& imgui);
void hide(imgui_sdl_backend& imgui);
bool is_shown(const imgui_sdl_backend& imgui);
bool is_hidden(const imgui_sdl_backend& imgui);

} // namespace kairos