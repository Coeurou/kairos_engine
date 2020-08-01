#pragma once

#include <window.h>

#define NAMEOF_ENUM_RANGE_MAX 512
#include <nameof.hpp>

#include <application.h>
#include <error.h>
#include <logger.h>
#include <messages.h>

namespace details
{
    // Callbacks define
    static void error_callback(int /*error*/, const char* description) {
        log_error(LoggerName::MISC, "Error: {}\n", description);
    }

    static void key_callback(native_window* window, int key, int /*scancode*/, int action, int /*mods*/) {
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        }
        send_message(NAMEOF_ENUM(static_cast<::key>(key)), NAMEOF_ENUM(static_cast<::action>(action)), MessageType::key_pressed);
    }

    void mouse_button_callback(native_window* /*window*/, int button, int action, int /*mods*/) {
        send_message(NAMEOF_ENUM(static_cast<mouse_button>(button)), NAMEOF_ENUM(static_cast<::action>(action)),
            MessageType::mouse_button_pressed);
    }

    static void framebuffer_resized_callback(native_window* /*window*/, int width, int height) {
        glViewport(0, 0, width, height);
        send_message(width, height, MessageType::window_resized);
    }

    void register_events_callback(native_window* main_window) {
        glfwSetErrorCallback(error_callback);
        glfwSetKeyCallback(main_window, key_callback);
        glfwSetMouseButtonCallback(main_window, mouse_button_callback);
        glfwSetFramebufferSizeCallback(main_window, framebuffer_resized_callback);
    }
}

window create_window(int width, int height, std::string_view title) {
    native_window* glfw_window = glfwCreateWindow(width, height, title.data(), nullptr, nullptr);
    ASSERT_MSG(glfw_window != nullptr, "[create_window] Window creation failed");

    details::register_events_callback(glfw_window);

    glfwMakeContextCurrent(glfw_window);

    return window{ glfw_window };
}

vec2i window::size() const {
    int framebuffer_width, framebuffer_height = 0;
    glfwGetFramebufferSize(application::native_window(), &framebuffer_width, &framebuffer_height);
    return { framebuffer_width, framebuffer_height };
}

void window::render() {
    
}