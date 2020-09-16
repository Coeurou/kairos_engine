#pragma once

struct ImDrawData;

class imgui_renderer {
public:
    virtual ~imgui_renderer() {}

    virtual bool setup() = 0;
    virtual void cleanup() = 0;
    virtual void render(ImDrawData* draw_data) = 0;
};