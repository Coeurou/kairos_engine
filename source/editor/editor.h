#pragma once

#include <memory>

#include <core/result.h>
#include <imgui_backend.h>
#include <imgui_console_command.h>
#include <imgui_renderer.h>

namespace kairos {

class editor {
  public:
    editor(imgui_backend backend, imgui_renderer renderer);
    void setup(window w);
    result<uint32> update();
    void process_event(const system_event& e);
    result<uint32> render();
    void cleanup();

  private:
      // editor shouldn't be limited to imgui, an abstraction (GUIRenderer, GUIBackend) will help, plus an editor has a collection of widgets it renders
    imgui_backend my_imgui_backend;
    imgui_renderer my_imgui_renderer;
    imgui_console_command my_imgui_console_command;
};

} // namespace kairos