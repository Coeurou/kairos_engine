#pragma once

#include <memory>

#include <application.h>
#include <imgui_backend.h>
#include <imgui_console_command.h>
#include <imgui_renderer.h>

class editor {
public:
    void setup();
    void update();
    void poll_event(const variant_event& e);
    void render();
    void cleanup();

private:
    std::unique_ptr<imgui_backend> my_imgui_backend;
    std::unique_ptr<imgui_renderer> my_imgui_renderer;
    imgui_console_command my_imgui_console_command;
};