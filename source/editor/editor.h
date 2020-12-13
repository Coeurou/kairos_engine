#pragma once

#include <memory>

#include <core/result.h>
#include <imgui_backend.h>
#include <imgui_console_command.h>
#include <imgui_renderer.h>

namespace kairos {

class editor {
  public:
    void setup();
    result<uint32> update();
    // void poll_event(const variant_event& e);
    result<uint32> render();
    void cleanup();

  private:
    std::unique_ptr<imgui_backend> my_imgui_backend;
    std::unique_ptr<imgui_renderer> my_imgui_renderer;
    imgui_console_command my_imgui_console_command;
};

} // namespace kairos