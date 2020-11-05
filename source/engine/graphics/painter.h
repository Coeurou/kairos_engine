#pragma once

#include <memory>

#include <graphics/abstract_2d_renderer.h>
#include <application/application.h>
#include <graphics/brush.h>
#include <graphics/pen.h>

class painter {
public:
    painter();
    painter(renderer_engine engine);
    ~painter();

    void draw_line(const linef& line);
    void draw_lines(const array<linef>& lines);
    void draw_lines(const array<std::pair<linef, color>>& colored_lines);

    void draw_rect(const rectf& rect);
    void draw_rects(const array<rectf>& rects);
    void draw_rects(const array<std::pair<rectf, color>>& colored_rects);
    void draw_rects(const array<std::pair<rectf, texture>>& textured_rects);

    void draw_sprite(const sprite& sprite);
    void draw_sprites(const array<sprite>& sprites);

    void draw_ellipse(const pointf& center, float rx, float ry);
    void draw_ellipse(const rectf& rect);
    void draw_ellipses(const array<rectf>& rects);
    void draw_ellipses(const array<std::pair<rectf, color>>& colored_ellipses);
    void draw_ellipses(const array<std::pair<rectf, texture>>& textured_ellipses);

    void draw_text(const pointf& pos, string_view text);

    void set_pen_color(const color& pen_color);
    void set_line_width(float line_width);
    void set_pen_opacity(float opacity);
    void set_brush_color(const color& fill_color);
    void set_brush_texture(int fill_texture_id);
    void set_brush_opacity(float opacity);

private:
    std::unique_ptr<abstract_renderer> my_renderer;
    pen my_pen{};
    brush my_brush{};
};