#pragma once

#include <window/window.h>

namespace kairos {

class imgui_backend {
  public:
    virtual ~imgui_backend() {}

    virtual bool setup(window window) = 0;
    virtual void cleanup() = 0;
    virtual void update() = 0;
    // virtual void process_event(const variant_event& event) = 0;

    bool is_imgui_visible = true;
};

} // namespace kairos