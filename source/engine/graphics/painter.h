#pragma once

#include <memory>

#include <application.h>
#include <2d_renderer_interface.h>
#include <brush.h>
#include <pen.h>

class painter {
public:
    painter();
    painter(renderer_engine engine);

    void draw_line(const linef& line);

    void draw_lines(const array<linef>& lines);
    void draw_lines(const array<std::pair<linef, color>>& colored_lines);

    void draw_rect(const rectf& rect);

    void draw_rects(const array<rectf>& rects);
    void draw_rects(const array<std::pair<rectf, color>>& colored_rects);
    void draw_rects(const array<std::pair<rectf, texture>>& textured_rects);

    void draw_ellipse(pointf center, float rx, float ry);
    void draw_ellipse(const rectf& rect);

    void draw_ellipses(const array<rectf>& rects);
    void draw_ellipses(const array<std::pair<rectf, color>>& colored_ellipses);
    void draw_ellipses(const array<std::pair<rectf, texture>>& textured_ellipses);

    void draw_text(pointf pos, string_view text);

    void set_pen_color(color pen_color);
    void set_line_width(float line_width);
    void set_pen_opacity(float opacity);
    void set_brush_color(color fill_color);
    void set_brush_texture(texture fill_texture);
    void set_brush_opacity(float opacity);

private:
    std::unique_ptr<renderer_interface> my_renderer;
    pen my_pen{};
    brush my_brush{};
};