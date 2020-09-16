#pragma once

#include <editor.h>

#include <algorithm>

#include <contract.h>
#include <imgui_opengl_renderer.h>
#include <imgui_sdl_backend.h>
#include <imgui_internal.h>
#include <command_parser.h>

void editor::setup() {
    expects(core_application::window() != nullptr, "Cannot setup the editor if no window available");

    if (core_application::is_initialized(implementation_type::SDL)) {
        my_imgui_backend = std::make_unique<imgui_sdl_backend>();
    }
    my_imgui_backend->setup(core_application::window());

    if (core_application::graphics_implementation() == renderer_engine::opengl ||
        core_application::graphics_implementation() == renderer_engine::opengl_es) {
        expects(core_application::gl_context() != nullptr, "Cannot setup the editor if no OpenGL context is created");
        my_imgui_renderer = std::make_unique<imgui_opengl_renderer>(core_application::gl_context());
    }
    my_imgui_renderer->setup();
}

void editor::update() {
    expects(my_imgui_backend != nullptr, "No imgui backend found, did you setup the editor ?");

    my_imgui_backend->update();
}

void editor::poll_event(const variant_event& e) {
    my_imgui_backend->process_event(e);
}

void editor::render() {
    expects(my_imgui_renderer != nullptr, "No imgui renderer found, did you setup the editor ?");

    ImGui::NewFrame();

    // draw console command
    if (my_imgui_backend->is_imgui_visible) {
        ImGui::ShowDemoWindow(&my_imgui_backend->is_imgui_visible);

        my_imgui_console_command.render(&my_imgui_backend->is_imgui_visible);
    }
    ImGui::Render();
    my_imgui_renderer->render(ImGui::GetDrawData());
}

void editor::cleanup() {
    expects(my_imgui_backend != nullptr, "No imgui backend found, did you setup the editor ?");
    expects(my_imgui_renderer != nullptr, "No imgui renderer found, did you setup the editor ?");

    my_imgui_backend->cleanup();
    my_imgui_renderer->cleanup();
}