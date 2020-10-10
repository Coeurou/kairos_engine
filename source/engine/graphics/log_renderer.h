#pragma once

#include <2d_renderer_interface.h>

class log_renderer : public renderer_interface {
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

    void draw_sprite(const sprite& sprite) override;
    void draw_sprites(const array<sprite>& sprites) override;

    void draw_ellipse(pointf center, float rx, float ry) override;
    void draw_ellipses(const array<rectf>& rects) override;
    void draw_ellipses(const array<std::pair<rectf, color>>& colored_ellipses) override;
    void draw_ellipses(const array<std::pair<rectf, texture>>& textured_ellipses) override;

    void draw_text(pointf pos, string_view text) override;

private:
    string my_color_value{ "" };
    string my_line_width_value{ "" };
    string my_pen_opacity_value{ "" };
    string my_fill_texture_value{ "" };
    string my_fill_color_value{ "" };
    string my_brush_opacity_value{ "" };
};