#pragma once

#include <application/command_line_parser.h>
#include <core/filesystem.h>
#include <core/types.h>

class opengl_context;
class window_interface;

enum class implementation_type {
    GLFW,
    SDL,
    NO_IMPL
};

enum class renderer_engine {
    opengl_es,
    opengl,
    no_render,
    count
};

class core_application {
public:
    static window_interface* window();

    static opengl_context* gl_context();

    static path get_application_dirpath();

    static array<argument> arguments();

    static void set_arguments(const parse_result* arguments);

    static bool init();

    static void exec();

    static void exit();

    static void process_events();

    static bool is_initialized(implementation_type type);

    static renderer_engine graphics_implementation();

    static void set_graphics_implementation(renderer_engine renderer);

private:
    static bool is_application_initialized;
    static renderer_engine our_graphics_impl;
};