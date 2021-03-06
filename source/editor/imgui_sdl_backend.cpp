#include <imgui_sdl_backend.h>

#include <SDL.h>
#include <SDL_syswm.h>
#if defined(__APPLE__)
#include <TargetConditionals.h>
#endif

#include <core/contract.h>
#include <core/event.h>

static const char* get_clipboard_text(void*) { return SDL_GetClipboardText(); }

static void set_clipboard_text(void*, const char* text) { SDL_SetClipboardText(text); }

namespace kairos {

void imgui_sdl_backend::process_event(const system_event& event) {
    ImGuiIO& io = ImGui::GetIO();

    const auto sign = [](int value) { return -1 + 2 * static_cast<int>(value > 0); };
    const auto on_key_event = [&](const keyboard_event& e) {
        int key = static_cast<int>(e.my_key);
        IM_ASSERT(key >= 0 && key < IM_ARRAYSIZE(io.KeysDown));
        io.KeysDown[key] = (e.my_action == action::press);
        io.KeyShift = ((SDL_GetModState() & KMOD_SHIFT) != 0);
        io.KeyCtrl = ((SDL_GetModState() & KMOD_CTRL) != 0);
        io.KeyAlt = ((SDL_GetModState() & KMOD_ALT) != 0);
#ifdef _WIN32
        io.KeySuper = false;
#else
        io.KeySuper = ((SDL_GetModState() & KMOD_GUI) != 0);
#endif
    };

    switch (event.my_type) {
    case event_type::mouse_wheel: {
        const auto& wheel_event = std::get<mouse_wheel_event>(event.my_internal);

        io.MouseWheelH += static_cast<float>(sign(wheel_event.my_scroll.x));
        io.MouseWheel += static_cast<float>(sign(wheel_event.my_scroll.y));
        break;
    }
    case event_type::mouse_button_press: {
        const auto& m = cast<mouse_event>(event);
        my_mouse_button_pressed[0] = m.my_button == mouse_button::left;
        my_mouse_button_pressed[1] = m.my_button == mouse_button::right;
        my_mouse_button_pressed[2] = m.my_button == mouse_button::middle;
        break;
    }
    case event_type::text_input: {
        const auto& text = cast<text_input_event>(event);
        io.AddInputCharactersUTF8(text.my_text.c_str());
        break;
    }
    case event_type::key_press: {
        on_key_event(cast<keyboard_event>(event));
        break;
    }
    case event_type::key_release: {
        const keyboard_event& k = cast<keyboard_event>(event);
        on_key_event(k);

        if (k.my_key == key::E && io.KeyCtrl) {
            is_imgui_visible = !is_imgui_visible;
        }
        else if (k.my_key == key::G && io.KeyCtrl) {
            io.BackendFlags ^= ImGuiBackendFlags_HasGamepad;
        }
        break;
    }
    }
}

bool imgui_sdl_backend::is_visible() const { return is_imgui_visible; }

void imgui_sdl_backend::set_visible(bool visibility) { is_imgui_visible = visibility; }

bool imgui_sdl_backend::setup(window window) {
    expects(SDL_WasInit(SDL_INIT_VIDEO) != 0,
            "Cannot use imgui if no SDL application is initialized");

    my_window = SDL_GetWindowFromID(id(window));
    expects(my_window != nullptr, "No SDL window found");

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls
    io.BackendFlags |=
        ImGuiBackendFlags_HasMouseCursors; // We can honor GetMouseCursor() values (optional)
    io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos; // We can honor io.WantSetMousePos requests
                                                         // (optional, rarely used)
    io.BackendFlags &= ~ImGuiBackendFlags_HasGamepad;
    io.BackendPlatformName = "imgui_kairos_sdl";

    // Keyboard mapping. ImGui will use those indices to peek into the io.KeysDown[] array.
    io.KeyMap[ImGuiKey_Tab] = to<key, int>(key::Tab);
    io.KeyMap[ImGuiKey_LeftArrow] = to<key, int>(key::Left);
    io.KeyMap[ImGuiKey_RightArrow] = to<key, int>(key::Right);
    io.KeyMap[ImGuiKey_UpArrow] = to<key, int>(key::Up);
    io.KeyMap[ImGuiKey_DownArrow] = to<key, int>(key::Down);
    io.KeyMap[ImGuiKey_PageUp] = to<key, int>(key::PageUp);
    io.KeyMap[ImGuiKey_PageDown] = to<key, int>(key::PageDown);
    io.KeyMap[ImGuiKey_Home] = to<key, int>(key::Home);
    io.KeyMap[ImGuiKey_End] = to<key, int>(key::End);
    io.KeyMap[ImGuiKey_Insert] = to<key, int>(key::Insert);
    io.KeyMap[ImGuiKey_Delete] = to<key, int>(key::Delete);
    io.KeyMap[ImGuiKey_Backspace] = to<key, int>(key::Backspace);
    io.KeyMap[ImGuiKey_Space] = to<key, int>(key::Space);
    io.KeyMap[ImGuiKey_Enter] = to<key, int>(key::Enter);
    io.KeyMap[ImGuiKey_Escape] = to<key, int>(key::Escape);
    io.KeyMap[ImGuiKey_KeyPadEnter] = to<key, int>(key::EnterKeyPad);
    io.KeyMap[ImGuiKey_A] = to<key, int>(key::A);
    io.KeyMap[ImGuiKey_C] = to<key, int>(key::C);
    io.KeyMap[ImGuiKey_V] = to<key, int>(key::V);
    io.KeyMap[ImGuiKey_X] = to<key, int>(key::X);
    io.KeyMap[ImGuiKey_Y] = to<key, int>(key::Y);
    io.KeyMap[ImGuiKey_Z] = to<key, int>(key::Z);

    io.SetClipboardTextFn = set_clipboard_text;
    io.GetClipboardTextFn = get_clipboard_text;
    io.ClipboardUserData = NULL;

    // Load mouse cursors
    my_mouse_cursors[ImGuiMouseCursor_Arrow] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW);
    my_mouse_cursors[ImGuiMouseCursor_TextInput] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_IBEAM);
    my_mouse_cursors[ImGuiMouseCursor_ResizeAll] =
        SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZEALL);
    my_mouse_cursors[ImGuiMouseCursor_ResizeNS] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZENS);
    my_mouse_cursors[ImGuiMouseCursor_ResizeEW] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZEWE);
    my_mouse_cursors[ImGuiMouseCursor_ResizeNESW] =
        SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZENESW);
    my_mouse_cursors[ImGuiMouseCursor_ResizeNWSE] =
        SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZENWSE);
    my_mouse_cursors[ImGuiMouseCursor_Hand] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND);
    my_mouse_cursors[ImGuiMouseCursor_NotAllowed] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_NO);

    // Check and store if we are on Wayland
    can_use_mouse_global_state = strncmp(SDL_GetCurrentVideoDriver(), "wayland", 7) != 0;

#ifdef _WIN32
    SDL_SysWMinfo wmInfo;
    SDL_VERSION(&wmInfo.version);
    SDL_GetWindowWMInfo(my_window, &wmInfo);
    io.ImeWindowHandle = wmInfo.info.win.window;
#endif

    return true;
}

void imgui_sdl_backend::cleanup() {
    my_window = nullptr;
    my_mouse_button_pressed = {false, false, false};
    for (auto cursor : my_mouse_cursors) {
        SDL_FreeCursor(cursor);
        cursor = nullptr;
    }
}

void imgui_sdl_backend::update() {
    ImGuiIO& io = ImGui::GetIO();
    IM_ASSERT(io.Fonts->IsBuilt() &&
              "Font atlas not built! It is generally built by the renderer back-end. Missing call "
              "to renderer _NewFrame() function? e.g. ImGui_ImplOpenGL3_NewFrame().");

    // Setup display size (every frame to accommodate for window resizing)
    vec2i window_size{0, 0};
    int display_w, display_h;
    if (SDL_GetWindowFlags(my_window) & ~SDL_WINDOW_MINIMIZED) {
        SDL_GetWindowSize(my_window, &window_size.x, &window_size.y);
    }
    SDL_GL_GetDrawableSize(my_window, &display_w, &display_h);
    io.DisplaySize = ImVec2(static_cast<float>(window_size.x), static_cast<float>(window_size.y));
    if (window_size.x > 0 && window_size.y > 0) {
        io.DisplayFramebufferScale = ImVec2(display_w / static_cast<float>(window_size.x),
                                            display_h / static_cast<float>(window_size.y));
    }

    update_mouse_keyboard();
    update_cursor();
    update_gamepads();
}

void imgui_sdl_backend::update_mouse_keyboard() {
    ImGuiIO& io = ImGui::GetIO();

    // Set OS mouse position if requested (rarely used, only when
    // ImGuiConfigFlags_NavEnableSetMousePos is enabled by user)
    if (io.WantSetMousePos) {
        SDL_WarpMouseInWindow(my_window, (int)io.MousePos.x, (int)io.MousePos.y);
    } else
        io.MousePos = ImVec2(-FLT_MAX, -FLT_MAX);

    int mx = 0;
    int my = 0;
    uint32 mouse_buttons = SDL_GetMouseState(&mx, &my);
    io.MouseDown[0] =
        my_mouse_button_pressed[0] ||
        (mouse_buttons & SDL_BUTTON(SDL_BUTTON_LEFT)) !=
            0; // If a mouse press event came, always pass it as "mouse held this frame", so we
               // don't miss click-release events that are shorter than 1 frame.
    io.MouseDown[1] =
        my_mouse_button_pressed[1] || (mouse_buttons & SDL_BUTTON(SDL_BUTTON_RIGHT)) != 0;
    io.MouseDown[2] =
        my_mouse_button_pressed[2] || (mouse_buttons & SDL_BUTTON(SDL_BUTTON_MIDDLE)) != 0;
    my_mouse_button_pressed[0] = my_mouse_button_pressed[1] = my_mouse_button_pressed[2] = false;

#if !defined(__EMSCRIPTEN__) && !defined(__ANDROID__) && !(defined(__APPLE__) && TARGET_OS_IOS)
    SDL_Window* focused_window = SDL_GetKeyboardFocus();
    if (my_window == focused_window) {
        if (can_use_mouse_global_state) {
            // SDL_GetMouseState() gives mouse position seemingly based on the last window
            // entered/focused(?) The creation of a new windows at runtime and SDL_CaptureMouse both
            // seems to severely mess up with that, so we retrieve that position globally. Won't use
            // this workaround when on Wayland, as there is no global mouse position.
            int wx, wy;
            SDL_GetWindowPosition(focused_window, &wx, &wy);
            SDL_GetGlobalMouseState(&mx, &my);
            mx -= wx;
            my -= wy;
        }
        io.MousePos = ImVec2((float)mx, (float)my);
    }

    bool any_mouse_button_down = ImGui::IsAnyMouseDown();
    SDL_CaptureMouse(any_mouse_button_down ? SDL_TRUE : SDL_FALSE);
#else
    if (SDL_GetWindowFlags(my_window->my_native_window) & SDL_WINDOW_INPUT_FOCUS)
        io.MousePos = ImVec2((float)mx, (float)my);
#endif
}

void imgui_sdl_backend::update_cursor() {
    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_NoMouseCursorChange) {
        return;
    }

    ImGuiMouseCursor imgui_cursor = ImGui::GetMouseCursor();
    if (io.MouseDrawCursor || imgui_cursor == ImGuiMouseCursor_None) {
        // Hide OS mouse cursor if imgui is drawing it or if it wants no cursor
        SDL_ShowCursor(SDL_FALSE);
    } else {
        // Show OS mouse cursor
        SDL_SetCursor(my_mouse_cursors[imgui_cursor] ? my_mouse_cursors[imgui_cursor]
                                                     : my_mouse_cursors[ImGuiMouseCursor_Arrow]);
        SDL_ShowCursor(SDL_TRUE);
    }
}

void imgui_sdl_backend::update_gamepads() {
    ImGuiIO& io = ImGui::GetIO();
    memset(io.NavInputs, 0, sizeof(io.NavInputs));
    if ((io.ConfigFlags & ImGuiConfigFlags_NavEnableGamepad) == 0)
        return;

    // Get gamepad
    SDL_GameController* game_controller = SDL_GameControllerOpen(0);
    if (!game_controller || (io.BackendFlags & ImGuiBackendFlags_HasGamepad) == 0) {
        io.BackendFlags &= ~ImGuiBackendFlags_HasGamepad;
        return;
    }

    // Update gamepad inputs
    const auto map_button = [&](size_t nav_input, SDL_GameControllerButton button) {
        io.NavInputs[nav_input] =
            (SDL_GameControllerGetButton(game_controller, button) != 0) ? 1.f : 0.f;
    };
    const auto map_analog = [&](size_t nav_input, SDL_GameControllerAxis axis, int v0, int v1) {
        float vn = static_cast<float>(SDL_GameControllerGetAxis(game_controller, axis) - v0) /
                   static_cast<float>(v1 - v0);
        if (vn > 1.0f)
            vn = 1.0f;
        if (vn > 0.0f && io.NavInputs[nav_input] < vn)
            io.NavInputs[nav_input] = vn;
    };
    const int thumb_dead_zone = 8000; // SDL_gamecontroller.h suggests using this value.
    map_button(ImGuiNavInput_Activate, SDL_CONTROLLER_BUTTON_A);              // Cross / A
    map_button(ImGuiNavInput_Cancel, SDL_CONTROLLER_BUTTON_B);                // Circle / B
    map_button(ImGuiNavInput_Menu, SDL_CONTROLLER_BUTTON_X);                  // Square / X
    map_button(ImGuiNavInput_Input, SDL_CONTROLLER_BUTTON_Y);                 // Triangle / Y
    map_button(ImGuiNavInput_DpadLeft, SDL_CONTROLLER_BUTTON_DPAD_LEFT);      // D-Pad Left
    map_button(ImGuiNavInput_DpadRight, SDL_CONTROLLER_BUTTON_DPAD_RIGHT);    // D-Pad Right
    map_button(ImGuiNavInput_DpadUp, SDL_CONTROLLER_BUTTON_DPAD_UP);          // D-Pad Up
    map_button(ImGuiNavInput_DpadDown, SDL_CONTROLLER_BUTTON_DPAD_DOWN);      // D-Pad Down
    map_button(ImGuiNavInput_FocusPrev, SDL_CONTROLLER_BUTTON_LEFTSHOULDER);  // L1 / LB
    map_button(ImGuiNavInput_FocusNext, SDL_CONTROLLER_BUTTON_RIGHTSHOULDER); // R1 / RB
    map_button(ImGuiNavInput_TweakSlow, SDL_CONTROLLER_BUTTON_LEFTSHOULDER);  // L1 / LB
    map_button(ImGuiNavInput_TweakFast, SDL_CONTROLLER_BUTTON_RIGHTSHOULDER); // R1 / RB
    map_analog(ImGuiNavInput_LStickLeft, SDL_CONTROLLER_AXIS_LEFTX, -thumb_dead_zone, -32768);
    map_analog(ImGuiNavInput_LStickRight, SDL_CONTROLLER_AXIS_LEFTX, +thumb_dead_zone, +32767);
    map_analog(ImGuiNavInput_LStickUp, SDL_CONTROLLER_AXIS_LEFTY, -thumb_dead_zone, -32767);
    map_analog(ImGuiNavInput_LStickDown, SDL_CONTROLLER_AXIS_LEFTY, +thumb_dead_zone, +32767);
}

void show(imgui_sdl_backend& imgui) { imgui.set_visible(true); }

void hide(imgui_sdl_backend& imgui) { imgui.set_visible(false); }

bool is_shown(const imgui_sdl_backend& imgui) { return imgui.is_visible(); }

bool is_hidden(const imgui_sdl_backend& imgui) { return !imgui.is_visible(); }

} // namespace kairos