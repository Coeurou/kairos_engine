#pragma once

#include <rect.h>
#include <sprite.h>
#include <texture.h>
#include <types.h>
#include <vertex.h>

enum class renderer_dirty_flag {
    pen_color,
    line_width,
    pen_opacity,
    brush_color,
    texture,
    brush_opacity,
    viewport,
    count
};

class brush;
class pen;

class abstract_renderer {
public:
    virtual ~abstract_renderer() {}

    virtual void setup() = 0;
    virtual void cleanup() = 0;

    virtual void update_state(renderer_dirty_flag flag, const variant& value) = 0;

    virtual void draw_line(const linef& line) = 0;
    virtual void draw_lines(const array<linef>& lines) = 0;
    virtual void draw_lines(const array<std::pair<linef, color>>& colored_lines) = 0;

    virtual void draw_rect(const rectf& rect) = 0;
    virtual void draw_rects(const array<rectf>& rects) = 0;
    virtual void draw_rects(const array<std::pair<rectf, color>>& colored_rects) = 0;
    virtual void draw_rects(const array<std::pair<rectf, texture>>& textured_rects) = 0;

    virtual void draw_sprite(const sprite& sprite) = 0;
    virtual void draw_sprites(const array<sprite>& sprites) = 0;

    virtual void draw_ellipse(const pointf& center, float rx, float ry) = 0;
    virtual void draw_ellipses(const array<rectf>& rects) = 0;
    virtual void draw_ellipses(const array<std::pair<rectf, color>>& colored_ellipses) = 0;
    virtual void draw_ellipses(const array<std::pair<rectf, texture>>& textured_ellipses) = 0;

    virtual void draw_text(const pointf& pos, string_view text) = 0;
};