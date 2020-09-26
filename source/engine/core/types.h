#pragma once

#include <array>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>
#include <unordered_map>
#include <variant>
#include <vector>

#include <fmt/format.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <SDL.h>

using vec2i = glm::ivec2;
using vec2f = glm::vec2;
using vec3f = glm::vec3;
using vec4f = glm::vec4;
using color = glm::vec4;
using mat4f = glm::mat4;
using int64 = std::int64_t;
using uint64 = std::uint64_t;
using int8 = char;
using uint8 = unsigned char;
using int32 = int;
using uint32 = std::uint32_t;
using sizef = vec2f;
using pointf = vec2f;
using linef = std::pair<vec2f, vec2f>;
template<class T> using array = std::vector<T>;
template<class T, size_t N> using static_array = std::array<T, N>;
template<class Key, class Value> using dictionary = std::unordered_map<Key, Value>;
using string = std::string;
using string_view = std::string_view;
using native_window = SDL_Window;
using variant = std::variant<int, uint32, float, size_t, int64, uint64, double, vec2f, vec2i, vec3f, string, string_view>;

enum class key : int {
    Unknown = SDL_SCANCODE_UNKNOWN,         //!< Unhandled key
    A = SDL_SCANCODE_A,                     //!< The A key
    B = SDL_SCANCODE_B,                     //!< The B key
    C = SDL_SCANCODE_C,                     //!< The C key
    D = SDL_SCANCODE_D,                     //!< The D key
    E = SDL_SCANCODE_E,                     //!< The E key
    F = SDL_SCANCODE_F,                     //!< The F key
    G = SDL_SCANCODE_G,                     //!< The G key
    H = SDL_SCANCODE_H,                     //!< The H key
    I = SDL_SCANCODE_I,                     //!< The I key
    J = SDL_SCANCODE_J,                     //!< The J key
    K = SDL_SCANCODE_K,                     //!< The K key
    L = SDL_SCANCODE_L,                     //!< The L key
    M = SDL_SCANCODE_M,                     //!< The M key
    N = SDL_SCANCODE_N,                     //!< The N key
    O = SDL_SCANCODE_O,                     //!< The O key
    P = SDL_SCANCODE_P,                     //!< The P key
    Q = SDL_SCANCODE_Q,                     //!< The Q key
    R = SDL_SCANCODE_R,                     //!< The R key
    S = SDL_SCANCODE_S,                     //!< The S key
    T = SDL_SCANCODE_T,                     //!< The T key
    U = SDL_SCANCODE_U,                     //!< The U key
    V = SDL_SCANCODE_V,                     //!< The V key
    W = SDL_SCANCODE_W,                     //!< The W key
    X = SDL_SCANCODE_X,                     //!< The X key
    Y = SDL_SCANCODE_Y,                     //!< The Y key
    Z = SDL_SCANCODE_Z,                     //!< The Z key
    Num0 = SDL_SCANCODE_0,                  //!< The 0 key
    Num1 = SDL_SCANCODE_1,                  //!< The 1 key
    Num2 = SDL_SCANCODE_2,                  //!< The 2 key
    Num3 = SDL_SCANCODE_3,                  //!< The 3 key
    Num4 = SDL_SCANCODE_4,                  //!< The 4 key
    Num5 = SDL_SCANCODE_5,                  //!< The 5 key
    Num6 = SDL_SCANCODE_6,                  //!< The 6 key
    Num7 = SDL_SCANCODE_7,                  //!< The 7 key
    Num8 = SDL_SCANCODE_8,                  //!< The 8 key
    Num9 = SDL_SCANCODE_9,                  //!< The 9 key
    Escape = SDL_SCANCODE_ESCAPE,           //!< The Escape key
    LControl = SDL_SCANCODE_LCTRL,          //!< The left Control key
    LShift = SDL_SCANCODE_LSHIFT,           //!< The left Shift key
    LAlt = SDL_SCANCODE_LALT,               //!< The left Alt key
    LSystem = SDL_SCANCODE_LGUI,            //!< The left OS specific key: window (Windows and Linux), apple
    //!< (MacOS X), ...
    RControl = SDL_SCANCODE_RCTRL,          //!< The right Control key
    RShift = SDL_SCANCODE_RSHIFT,           //!< The right Shift key
    RAlt = SDL_SCANCODE_RALT,               //!< The right Alt key
    RSystem = SDL_SCANCODE_RGUI,            //!< The right OS specific key: window (Windows and Linux),
    //!< apple (MacOS X), ...
    Menu = SDL_SCANCODE_MENU,               //!< The Menu key
    LBracket = SDL_SCANCODE_LEFTBRACKET,    //!< The [ key
    RBracket = SDL_SCANCODE_RIGHTBRACKET,   //!< The ] key
    Semicolon = SDL_SCANCODE_SEMICOLON,     //!< The ; key
    Comma = SDL_SCANCODE_COMMA,             //!< The , key
    Period = SDL_SCANCODE_PERIOD,           //!< The . key
    Slash = SDL_SCANCODE_SLASH,             //!< The / key
    Backslash = SDL_SCANCODE_BACKSLASH,     //!< The \ key
    Equal = SDL_SCANCODE_EQUALS,            //!< The = key
    Quote = SDL_SCANCODE_APOSTROPHE,        //!< The - key (hyphen)
    Space = SDL_SCANCODE_SPACE,             //!< The Space key
    Enter = SDL_SCANCODE_RETURN,            //!< The Enter/Return keys
    Backspace = SDL_SCANCODE_BACKSPACE,     //!< The Backspace key
    Tab = SDL_SCANCODE_TAB,                 //!< The Tabulation key
    PageUp = SDL_SCANCODE_PAGEUP,           //!< The Page up key
    PageDown = SDL_SCANCODE_PAGEDOWN,       //!< The Page down key
    End = SDL_SCANCODE_END,                 //!< The End key
    Home = SDL_SCANCODE_HOME,               //!< The Home key
    Insert = SDL_SCANCODE_INSERT,           //!< The Insert key
    Delete = SDL_SCANCODE_DELETE,           //!< The Delete key
    Add = SDL_SCANCODE_KP_PLUS,             //!< The + key
    Subtract = SDL_SCANCODE_KP_MINUS,       //!< The - key (minus, usually from numpad)
    Multiply = SDL_SCANCODE_KP_MULTIPLY,    //!< The * key
    Divide = SDL_SCANCODE_KP_DIVIDE,        //!< The / key
    Decimal = SDL_SCANCODE_KP_DECIMAL,      //!< The . key
    EnterKeyPad = SDL_SCANCODE_KP_ENTER,    //!< The Enter key on pad
    EqualKeyPad = SDL_SCANCODE_KP_EQUALS,   //!< The Equal key on pad
    Left = SDL_SCANCODE_LEFT,               //!< Left arrow
    Right = SDL_SCANCODE_RIGHT,             //!< Right arrow
    Up = SDL_SCANCODE_UP,                   //!< Up arrow
    Down = SDL_SCANCODE_DOWN,               //!< Down arrow
    Numpad0 = SDL_SCANCODE_KP_0,            //!< The numpad 0 key
    Numpad1 = SDL_SCANCODE_KP_1,            //!< The numpad 1 key
    Numpad2 = SDL_SCANCODE_KP_2,            //!< The numpad 2 key
    Numpad3 = SDL_SCANCODE_KP_3,            //!< The numpad 3 key
    Numpad4 = SDL_SCANCODE_KP_4,            //!< The numpad 4 key
    Numpad5 = SDL_SCANCODE_KP_5,            //!< The numpad 5 key
    Numpad6 = SDL_SCANCODE_KP_6,            //!< The numpad 6 key
    Numpad7 = SDL_SCANCODE_KP_7,            //!< The numpad 7 key
    Numpad8 = SDL_SCANCODE_KP_8,            //!< The numpad 8 key
    Numpad9 = SDL_SCANCODE_KP_9,            //!< The numpad 9 key
    F1 = SDL_SCANCODE_F1,                   //!< The F1 key
    F2 = SDL_SCANCODE_F2,                   //!< The F2 key
    F3 = SDL_SCANCODE_F3,                   //!< The F3 key
    F4 = SDL_SCANCODE_F4,                   //!< The F4 key
    F5 = SDL_SCANCODE_F5,                   //!< The F5 key
    F6 = SDL_SCANCODE_F6,                   //!< The F6 key
    F7 = SDL_SCANCODE_F7,                   //!< The F7 key
    F8 = SDL_SCANCODE_F8,                   //!< The F8 key
    F9 = SDL_SCANCODE_F9,                   //!< The F9 key
    F10 = SDL_SCANCODE_F10,                 //!< The F10 key
    F11 = SDL_SCANCODE_F11,                 //!< The F11 key
    F12 = SDL_SCANCODE_F12,                 //!< The F12 key
    F13 = SDL_SCANCODE_F13,                 //!< The F13 key
    F14 = SDL_SCANCODE_F14,                 //!< The F14 key
    F15 = SDL_SCANCODE_F15,                 //!< The F15 key
    Pause = SDL_SCANCODE_PAUSE,             //!< The Pause key
    CapsLock = SDL_SCANCODE_CAPSLOCK,       //!< The CAPS LOCK key
    ScrollLock = SDL_SCANCODE_SCROLLLOCK,   //!< The Scroll lock key
    NumLock = SDL_SCANCODE_NUMLOCKCLEAR,    //!< The NUM LOCK key
    Minus = SDL_SCANCODE_MINUS,             //!< The minus key
    GraveAccent = SDL_SCANCODE_GRAVE,       //!< `

    KeyCount = SDL_NUM_SCANCODES,           //!< Keep last -- the total number of keyboard keys
};

enum class mouse_button {
    left = SDL_BUTTON_LEFT,
    right = SDL_BUTTON_RIGHT,
    middle = SDL_BUTTON_MIDDLE,
    button_4 = SDL_BUTTON_X1,
    button_5 = SDL_BUTTON_X2,
    button_6 = 6,
    button_7 = 7,
    last_button = 8
};

enum class action {
    press = SDL_PRESSED,
    release = SDL_RELEASED,
    repeat = 2,
    count = 3
};

template<class Enum, class Integral, typename = std::enable_if_t<std::is_integral_v<Integral>>> Integral to(Enum value) {
    return static_cast<Integral>(value);
}

struct clock {
    double delta_time = 0.0;
    double elapsed_time = 0.0;

    static double get_time() { return static_cast<double>(SDL_GetTicks()); }
};