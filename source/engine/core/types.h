#pragma once

#include <array>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>
#include <variant>
#include <vector>

#include <fmt/format.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

using vec2i = glm::ivec2;
using vec2f = glm::vec2;
using vec3f = glm::vec3;
using vec4f = glm::vec4;
using color = glm::vec3;
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
using string = std::string;
using string_view = std::string_view;
using native_window = GLFWwindow;
using variant = std::variant<int, float, size_t, int64, double, vec2f, vec2i, vec3f, string, string_view>;

enum class key : int {
    Unknown = GLFW_KEY_UNKNOWN,       //!< Unhandled key
    A = GLFW_KEY_A,                   //!< The A key
    B = GLFW_KEY_B,                   //!< The B key
    C = GLFW_KEY_C,                   //!< The C key
    D = GLFW_KEY_D,                   //!< The D key
    E = GLFW_KEY_E,                   //!< The E key
    F = GLFW_KEY_F,                   //!< The F key
    G = GLFW_KEY_G,                   //!< The G key
    H = GLFW_KEY_H,                   //!< The H key
    I = GLFW_KEY_I,                   //!< The I key
    J = GLFW_KEY_J,                   //!< The J key
    K = GLFW_KEY_K,                   //!< The K key
    L = GLFW_KEY_L,                   //!< The L key
    M = GLFW_KEY_M,                   //!< The M key
    N = GLFW_KEY_N,                   //!< The N key
    O = GLFW_KEY_O,                   //!< The O key
    P = GLFW_KEY_P,                   //!< The P key
    Q = GLFW_KEY_Q,                   //!< The Q key
    R = GLFW_KEY_R,                   //!< The R key
    S = GLFW_KEY_S,                   //!< The S key
    T = GLFW_KEY_T,                   //!< The T key
    U = GLFW_KEY_U,                   //!< The U key
    V = GLFW_KEY_V,                   //!< The V key
    W = GLFW_KEY_W,                   //!< The W key
    X = GLFW_KEY_X,                   //!< The X key
    Y = GLFW_KEY_Y,                   //!< The Y key
    Z = GLFW_KEY_Z,                   //!< The Z key
    Num0 = GLFW_KEY_0,                //!< The 0 key
    Num1 = GLFW_KEY_1,                //!< The 1 key
    Num2 = GLFW_KEY_2,                //!< The 2 key
    Num3 = GLFW_KEY_3,                //!< The 3 key
    Num4 = GLFW_KEY_4,                //!< The 4 key
    Num5 = GLFW_KEY_5,                //!< The 5 key
    Num6 = GLFW_KEY_6,                //!< The 6 key
    Num7 = GLFW_KEY_7,                //!< The 7 key
    Num8 = GLFW_KEY_8,                //!< The 8 key
    Num9 = GLFW_KEY_9,                //!< The 9 key
    Escape = GLFW_KEY_ESCAPE,         //!< The Escape key
    LControl = GLFW_KEY_LEFT_CONTROL, //!< The left Control key
    LShift = GLFW_KEY_LEFT_SHIFT,     //!< The left Shift key
    LAlt = GLFW_KEY_LEFT_ALT,         //!< The left Alt key
    LSystem = GLFW_KEY_LEFT_SUPER, //!< The left OS specific key: window (Windows and Linux), apple
    //!< (MacOS X), ...
    RControl = GLFW_KEY_RIGHT_CONTROL, //!< The right Control key
    RShift = GLFW_KEY_RIGHT_SHIFT,     //!< The right Shift key
    RAlt = GLFW_KEY_RIGHT_ALT,         //!< The right Alt key
    RSystem = GLFW_KEY_RIGHT_SUPER,    //!< The right OS specific key: window (Windows and Linux),
    //!< apple (MacOS X), ...
    Menu = GLFW_KEY_MENU,              //!< The Menu key
    LBracket = GLFW_KEY_LEFT_BRACKET,  //!< The [ key
    RBracket = GLFW_KEY_RIGHT_BRACKET, //!< The ] key
    Semicolon = GLFW_KEY_SEMICOLON,    //!< The ; key
    Comma = GLFW_KEY_COMMA,            //!< The , key
    Period = GLFW_KEY_PERIOD,          //!< The . key
    Slash = GLFW_KEY_SLASH,            //!< The / key
    Backslash = GLFW_KEY_BACKSLASH,    //!< The \ key
    Equal = GLFW_KEY_EQUAL,            //!< The = key
    Quote = GLFW_KEY_APOSTROPHE,       //!< The - key (hyphen)
    Space = GLFW_KEY_SPACE,            //!< The Space key
    Enter = GLFW_KEY_ENTER,            //!< The Enter/Return keys
    Backspace = GLFW_KEY_BACKSPACE,    //!< The Backspace key
    Tab = GLFW_KEY_TAB,                //!< The Tabulation key
    PageUp = GLFW_KEY_PAGE_UP,         //!< The Page up key
    PageDown = GLFW_KEY_PAGE_DOWN,     //!< The Page down key
    End = GLFW_KEY_END,                //!< The End key
    Home = GLFW_KEY_HOME,              //!< The Home key
    Insert = GLFW_KEY_INSERT,          //!< The Insert key
    Delete = GLFW_KEY_DELETE,          //!< The Delete key
    Add = GLFW_KEY_KP_ADD,             //!< The + key
    Subtract = GLFW_KEY_KP_SUBTRACT,   //!< The - key (minus, usually from numpad)
    Multiply = GLFW_KEY_KP_MULTIPLY,   //!< The * key
    Divide = GLFW_KEY_KP_DIVIDE,       //!< The / key
    Decimal = GLFW_KEY_KP_DECIMAL,     //!< The . key
    EnterKeyPad = GLFW_KEY_KP_ENTER,   //!< The Enter key on pad
    EqualKeyPad = GLFW_KEY_KP_EQUAL,   //!< The Equal key on pad
    Left = GLFW_KEY_LEFT,              //!< Left arrow
    Right = GLFW_KEY_RIGHT,            //!< Right arrow
    Up = GLFW_KEY_UP,                  //!< Up arrow
    Down = GLFW_KEY_DOWN,              //!< Down arrow
    Numpad0 = GLFW_KEY_KP_0,           //!< The numpad 0 key
    Numpad1 = GLFW_KEY_KP_1,           //!< The numpad 1 key
    Numpad2 = GLFW_KEY_KP_2,           //!< The numpad 2 key
    Numpad3 = GLFW_KEY_KP_3,           //!< The numpad 3 key
    Numpad4 = GLFW_KEY_KP_4,           //!< The numpad 4 key
    Numpad5 = GLFW_KEY_KP_5,           //!< The numpad 5 key
    Numpad6 = GLFW_KEY_KP_6,           //!< The numpad 6 key
    Numpad7 = GLFW_KEY_KP_7,           //!< The numpad 7 key
    Numpad8 = GLFW_KEY_KP_8,           //!< The numpad 8 key
    Numpad9 = GLFW_KEY_KP_9,           //!< The numpad 9 key
    F1 = GLFW_KEY_F1,                  //!< The F1 key
    F2 = GLFW_KEY_F2,                  //!< The F2 key
    F3 = GLFW_KEY_F3,                  //!< The F3 key
    F4 = GLFW_KEY_F4,                  //!< The F4 key
    F5 = GLFW_KEY_F5,                  //!< The F5 key
    F6 = GLFW_KEY_F6,                  //!< The F6 key
    F7 = GLFW_KEY_F7,                  //!< The F7 key
    F8 = GLFW_KEY_F8,                  //!< The F8 key
    F9 = GLFW_KEY_F9,                  //!< The F9 key
    F10 = GLFW_KEY_F10,                //!< The F10 key
    F11 = GLFW_KEY_F11,                //!< The F11 key
    F12 = GLFW_KEY_F12,                //!< The F12 key
    F13 = GLFW_KEY_F13,                //!< The F13 key
    F14 = GLFW_KEY_F14,                //!< The F14 key
    F15 = GLFW_KEY_F15,                //!< The F15 key
    Pause = GLFW_KEY_PAUSE,            //!< The Pause key
    CapsLock = GLFW_KEY_CAPS_LOCK,     //!< The CAPS LOCK key
    ScrollLock = GLFW_KEY_SCROLL_LOCK, //!< The Scroll lock key
    NumLock = GLFW_KEY_NUM_LOCK,       //!< The NUM LOCK key
    NON_US_1 = GLFW_KEY_WORLD_1,       //!< non-US #1
    NON_US_2 = GLFW_KEY_WORLD_2,       //!< non-US #2
    Minus = GLFW_KEY_MINUS,            //!< The minus key
    GraveAccent = GLFW_KEY_GRAVE_ACCENT, //!< `

    KeyCount = GLFW_KEY_LAST, //!< Keep last -- the total number of keyboard keys
};

enum class mouse_button {
    left = GLFW_MOUSE_BUTTON_LEFT,
    right = GLFW_MOUSE_BUTTON_RIGHT,
    middle = GLFW_MOUSE_BUTTON_MIDDLE,
    button_4 = GLFW_MOUSE_BUTTON_4,
    button_5 = GLFW_MOUSE_BUTTON_5,
    button_6 = GLFW_MOUSE_BUTTON_6,
    button_7 = GLFW_MOUSE_BUTTON_7,
    last_button = GLFW_MOUSE_BUTTON_LAST
};

enum class action {
    press = GLFW_PRESS,
    release = GLFW_RELEASE,
    repeat = GLFW_REPEAT,
    count = 3
};

struct clock {
    double delta_time = 0.0;
    double elapsed_time = 0.0;

    static double get_time() { return glfwGetTime(); }
};

template<class T> string to_string(T value) {
    return std::to_string(value);
}

template<> inline string to_string(string s) {
    return s;
}

template<> inline string to_string(string_view s) {
    return s.data();
}

template<> inline string to_string(vec2f v) {
    return fmt::format("vector 2D - x: {}, y: {}", v.x, v.y);
}

template<> inline string to_string(vec2i v) {
    return fmt::format("vector 2D - x: {}, y: {}", v.x, v.y);
}

template<> inline string to_string(vec3f v) {
    return fmt::format("vector 3D - x: {}, y: {}, z: {}", v.x, v.y, v.z);
}

template<> inline string to_string(vec4f v) {
    return fmt::format("vector 3D - x: {}, y: {}, z: {}, w: {}", v.x, v.y, v.z, v.w);
}

template<> inline string to_string(variant v) {
    return std::visit([](auto&& arg) {
        return to_string(arg);
        }, v);
}