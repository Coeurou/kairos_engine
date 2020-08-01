#include <application.h>

#include <messages.h>

application* app = nullptr;

void application::set_application(application* application) {
    app = application;
}

native_window* application::native_window() {
    return app->main_window.native_window;
}

path application::get_application_dirpath() {
    return current_path();
}

array<argument> application::arguments() {
    if (app->parsed_arguments != nullptr) {
        return app->parsed_arguments->arguments();
    }
    else {
        return array<argument>();
    }
}

void application::set_arguments(const parse_result* arguments) {
    app->parsed_arguments = arguments;
}

void application::exec() {
    while (!glfwWindowShouldClose(native_window())) {
        dispatch_messages();
        frame_number++;
        process_events();
        app->gl_context.swap_buffers();
    }
    glfwDestroyWindow(native_window());
    glfwTerminate();
}

void application::exit() {
    glfwSetWindowShouldClose(native_window(), GLFW_TRUE);
}

void application::process_events() {
    glfwPollEvents();
}