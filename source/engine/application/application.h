#pragma once

#include <variant>
#include <SDL.h>

#include <command_line_parser.h>
#include <core_application.h>
#include <opengl_context.h>
#include <window.h>

using variant_event = std::variant<SDL_Event, std::monostate>;

class application {
public:
    friend class core_application;

    enum class state {
        active,
        inactive,
        suspended,
        hidden
    };

    string my_name{ "" };
    string my_organisation{ "" };
    string my_version{ "" };
    uint64 my_session_id{ 0 };
    std::unique_ptr<window_interface> my_main_window;
    std::unique_ptr<opengl_context> my_gl_context;
    array<std::function<void(void)>> my_processes;
    array<std::function<void(const variant_event&)>> my_events_callbacks;
    //clipboard my_clipboard;
    //cursor my_cursor;

    virtual ~application() {}
    virtual bool init() = 0;
    virtual void exec() = 0;
    virtual void exit() = 0;
    virtual array<variant_event> process_events() = 0;

protected:
    const parse_result* parsed_arguments = nullptr;
    implementation_type my_type = implementation_type::NO_IMPL;
};

// Global variables
inline size_t frame_number{ 0 };
inline application* our_application = nullptr;

