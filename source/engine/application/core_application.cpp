#include <core_application.h>

#include <application.h>
#include <contract.h>
#include <opengl_context.h>
#include <window.h>

bool core_application::is_application_initialized = false;
renderer_engine core_application::our_graphics_impl = renderer_engine::no_render;

window_interface* core_application::window() {
    return our_application->my_main_window.get();
}

opengl_context* core_application::gl_context() {
    return our_application->my_gl_context.get();
}

path core_application::get_application_dirpath() {
    return filesystem::current_path();
}

array<argument> core_application::arguments() {
    if (our_application->parsed_arguments != nullptr) {
        const auto args = our_application->parsed_arguments->arguments();
        return array<argument>(args.begin(), args.end());
    }
    else {
        return array<argument>();
    }
}

void core_application::set_arguments(const parse_result* arguments) {
    our_application->parsed_arguments = arguments;
}

bool core_application::init() {
    is_application_initialized = our_application->init();
    ensures(is_application_initialized, "Couldn't initialized application, see implementation for more details");
    return is_application_initialized;
}

void core_application::exec() {
    auto main_window = our_application->my_main_window.get();
    while (!main_window->should_close()) {
        process_events();

        main_window->start_frame();
        our_application->exec();
        for (const auto& callback : our_application->my_execution_callbacks) {
            callback();
        }
        main_window->end_frame();
    }
}

void core_application::exit() {
    our_application->exit();
}

void core_application::process_events() {
    const auto polled_events = our_application->process_events();
    for (const auto& e : polled_events) {
        for (const auto& on_event : our_application->my_events_callbacks) {
            on_event(e);
        }
    }
}

bool core_application::is_initialized(implementation_type type) {
    return type == our_application->my_type && is_application_initialized;
}

renderer_engine core_application::graphics_implementation() {
    return our_graphics_impl;
}

void core_application::set_graphics_implementation(renderer_engine renderer) {
    our_graphics_impl = renderer;
}