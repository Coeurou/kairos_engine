#pragma once

#include <pen.h>
#include <rect.h>
#include <texture.h>
#include <types.h>

class painter {
public:
    void draw_line(const linef& line);

    void draw_lines(const array<linef>& lines);
    void draw_lines(const array<std::pair<linef, color>>& colored_lines);

    void draw_rect(const rectf& rect);

    void draw_rects(const array<rectf>& rect);
    void draw_rects(const array<std::pair<rectf, color>>& colored_rects);
    void draw_rects(const array<std::pair<rectf, texture>>& textured_rects);

    void draw_ellipse(pointf center, float rx, float ry);
    void draw_ellipse(const rectf& rect);

    void draw_ellipses(const array<rectf>& rect);
    void draw_ellipses(const array<std::pair<rectf, color>>& colored_ellipses);
    void draw_ellipses(const array<std::pair<rectf, texture>>& textured_ellipses);

    void draw_text(pointf pos, string_view text);

    pen pen{};
    //brush brush{};
};