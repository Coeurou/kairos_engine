#pragma once

#include <editor.h>

#include <algorithm>

#include <core/contract.h>
#include <imgui_opengl_renderer.h>
#include <imgui.h>
#include <command_parser.h>

namespace kairos {

editor::editor(imgui_backend backend, imgui_renderer renderer) : my_imgui_backend(std::move(backend)), my_imgui_renderer(std::move(renderer)) {}

void editor::setup(window w) {
    my_imgui_backend.setup(w);
    my_imgui_renderer.setup();
}

result<uint32> editor::update() {
    my_imgui_backend.update();

    return result<uint32>(EXIT_SUCCESS);
}

void editor::process_event(const system_event& e) { my_imgui_backend.process_event(e); }

result<uint32> editor::render() {
    ImGui::NewFrame();

    // draw console command
    bool is_visible = is_shown(my_imgui_backend);
    if (is_visible) {
        ImGui::ShowDemoWindow(&is_visible);

        my_imgui_console_command.render(&is_visible);
    }

    if (is_visible) {
        show(my_imgui_backend);
    }
    else {
        hide(my_imgui_backend);
    }

    // TODO draw menu bar with help (key shortcuts etc...)

    ImGui::Render();
    my_imgui_renderer.render(ImGui::GetDrawData());
    // should verify OpenGL error
    return result<uint32>(EXIT_SUCCESS);
}

void editor::cleanup() {
    my_imgui_backend.cleanup();
    my_imgui_renderer.cleanup();
}

} // namespace kairos