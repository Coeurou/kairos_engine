#pragma once

#include <application.h>

class window_interface;

class imgui_backend {
public:
    virtual ~imgui_backend() {}

    virtual bool setup(window_interface* window) = 0;
    virtual void cleanup() = 0;
    virtual void update() = 0;
    virtual void process_event(const variant_event& event) = 0;

    bool is_imgui_visible = true;
};