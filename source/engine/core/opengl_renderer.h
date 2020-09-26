#pragma once

#include <2d_renderer_interface.h>
#include <material.h>
#include <opengl_draw_data.h>

class opengl_renderer : public renderer_interface {
public:
    void setup() override;
    void cleanup() override;

    void update_state(renderer_dirty_flag flag, const variant& value) override;

    void draw_line(const linef& line) override;

    void draw_lines(const array<linef>& lines) override;
    void draw_lines(const array<std::pair<linef, color>>& colored_lines) override;

    void draw_rect(const rectf& rect) override;

    void draw_rects(const array<rectf>& rects) override;
    void draw_rects(const array<std::pair<rectf, color>>& colored_rects) override;
    void draw_rects(const array<std::pair<rectf, texture>>& textured_rects) override;

    void draw_ellipse(pointf center, float rx, float ry) override;

    void draw_ellipses(const array<rectf>& rects) override;
    void draw_ellipses(const array<std::pair<rectf, color>>& colored_ellipses) override;
    void draw_ellipses(const array<std::pair<rectf, texture>>& textured_ellipses) override;

    void draw_text(pointf pos, string_view text) override;

    // start_idx & end_idx are the range of indices in the IBO where the draw data are located. 
    // it is useful for rendering only one shape at the time
    void render(material material, uint32 start_idx, uint32 end_idx) const;

private:
    float my_line_width_value{ 0.f };
    float my_pen_opacity_value{ 0.f };
    float my_brush_opacity_value{ 0.f };
    color my_fill_color_value{ 0.f };
    color my_color_value{ 0.f };
    string my_fill_texture_value{ "" };

    dictionary<string, opengl_draw_data> my_draw_data;
};

inline void check_gl_error() {
    auto error = glGetError();
    ensures(error == GL_NO_ERROR, fmt::format("Last GL error: {}", error));
}