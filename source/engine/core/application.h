#pragma once

#include <command_line_parser.h>
#include <filesystem.h>
#include <opengl_context.h>
#include <types.h>
#include <window.h>

class application {
public:
    enum class state {
        active,
        inactive,
        suspended,
        hidden
    };

    string name{ "" };
    string organisation{ "" };
    string version{ "" };
    uint64 session_id{ 0 };
    window main_window{};
    opengl_context gl_context{};
    //clipboard clipboard;
    //cursor cursor;

    static void set_application(application* application);
    static native_window* native_window();
    static path get_application_dirpath();
    static array<argument> arguments();
    static void set_arguments(const parse_result* arguments);
    static void exec();
    static void exit();
    static void process_events();

private:
    const parse_result* parsed_arguments = nullptr;
};

// Global variables
inline size_t frame_number{ 0 };
