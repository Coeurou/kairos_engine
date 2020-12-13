#pragma once

#include <editor.h>

#include <algorithm>

#include <core/contract.h>
#include <imgui_opengl_renderer.h>
#include <imgui_sdl_backend.h>
#include <imgui_internal.h>
#include <command_parser.h>

namespace kairos {

void editor::setup() {
    /*expects(core_application::window() != nullptr, "Cannot setup the editor if no window
    available");

    if (core_application::is_initialized(windowing_implementation::SDL)) {
        my_imgui_backend = std::make_unique<imgui_sdl_backend>();
    }
    my_imgui_backend->setup(core_application::window());

    if (core_application::graphics_implementation() == renderer_engine::opengl ||
        core_application::graphics_implementation() == renderer_engine::opengl_es) {
        expects(core_application::gl_context() != nullptr, "Cannot setup the editor if no OpenGL
    context is created"); my_imgui_renderer =
    std::make_unique<imgui_opengl_renderer>(core_application::gl_context());
    }
    my_imgui_renderer->setup();*/
}

result<uint32> editor::update() {
    if (my_imgui_backend != nullptr) {
        // should retrieve facade::os_events and call my_imgui_backend->process_event(e)
        my_imgui_backend->update();

        return result<uint32>(EXIT_SUCCESS);
    } else {
        log_error(LoggerName::EDITOR, "No imgui backend found, did you setup the editor ?");
        return result<uint32>(error_type::uninitialized);
    }
}

/*void editor::poll_event(const variant_event& e) {
    my_imgui_backend->process_event(e);
}*/

result<uint32> editor::render() {
    if (my_imgui_renderer != nullptr) {

        ImGui::NewFrame();

        // draw console command
        if (my_imgui_backend->is_imgui_visible) {
            ImGui::ShowDemoWindow(&my_imgui_backend->is_imgui_visible);

            my_imgui_console_command.render(&my_imgui_backend->is_imgui_visible);
        }
        ImGui::Render();
        my_imgui_renderer->render(ImGui::GetDrawData());
        // should verify OpenGL error
        return result<uint32>(EXIT_SUCCESS);
    } else {
        log_error(LoggerName::EDITOR, "No imgui renderer found, did you setup the editor ?");
        return result<uint32>(error_type::uninitialized);
    }
}

void editor::cleanup() {
    expects(my_imgui_backend != nullptr, "No imgui backend found, did you setup the editor ?");
    expects(my_imgui_renderer != nullptr, "No imgui renderer found, did you setup the editor ?");

    my_imgui_backend->cleanup();
    my_imgui_renderer->cleanup();
}

} // namespace kairos