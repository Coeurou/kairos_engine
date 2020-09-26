#pragma once

#include <painter.h>
#include <logger.h>
#include <makable.h>
#include <kairos_lib.h>
#include <log_renderer.h>
#include <opengl_renderer.h>
#include <rect.h>

painter::painter() : my_renderer(make<log_renderer>()) {}

painter::painter(renderer_engine engine) {
    if (engine == renderer_engine::opengl) {
        my_renderer = make<opengl_renderer>();
        // TODO  this should be an application scope variable for renderer which is setup by api->video->setup
        my_renderer->setup();
    }
}

painter::~painter() {
    my_renderer->cleanup();
}

void painter::draw_line(const linef& line) {
    my_renderer->draw_line(line);
}

void painter::draw_lines(const array<linef>& lines) {
    my_renderer->draw_lines(lines);
}

void painter::draw_lines(const array<std::pair<linef, color>>& colored_lines) {
    my_renderer->draw_lines(colored_lines);
}

void painter::draw_rect(const rectf& rect) {
    my_renderer->draw_rect(rect);
}

void painter::draw_rects(const array<rectf>& rects) {
    my_renderer->draw_rects(rects);
}

void painter::draw_rects(const array<std::pair<rectf, color>>& colored_rects) {
    my_renderer->draw_rects(colored_rects);
}

void painter::draw_rects(const array<std::pair<rectf, texture>>& textured_rects) {
    my_renderer->draw_rects(textured_rects);
}

void painter::draw_ellipse(pointf center, float rx, float ry) {
    my_renderer->draw_ellipse(center, rx, ry);
}

void painter::draw_ellipse(const rectf& rect) {
    draw_ellipse(rect.center(), rect.width(), rect.height());
}

void painter::draw_ellipses(const array<rectf>& rects) {
    my_renderer->draw_ellipses(rects);
}

void painter::draw_ellipses(const array<std::pair<rectf, color>>& colored_ellipses) {
    my_renderer->draw_ellipses(colored_ellipses);
}

void painter::draw_ellipses(const array<std::pair<rectf, texture>>& textured_ellipses) {
    my_renderer->draw_ellipses(textured_ellipses);
}

void painter::draw_text(pointf pos, string_view text) {
    my_renderer->draw_text(pos, text);
}

void painter::set_canvas(canvas* canvas) {
    my_canvas = canvas;
    if (canvas != nullptr) {
        my_renderer->update_state(renderer_dirty_flag::viewport, to_string(canvas->my_viewport));
    }
}

void painter::set_pen_color(color pen_color) {
    my_pen.my_color = pen_color;
    my_renderer->update_state(renderer_dirty_flag::pen_color, static_cast<vec3f>(my_pen.my_color));
}

void painter::set_line_width(float line_width) {
    my_pen.my_line_width = line_width;
    my_renderer->update_state(renderer_dirty_flag::line_width, static_cast<float>(my_pen.my_line_width));
}

void painter::set_pen_opacity(float opacity) {
    my_pen.my_opacity = opacity;
    my_renderer->update_state(renderer_dirty_flag::pen_opacity, static_cast<float>(my_pen.my_opacity));
}

void painter::set_brush_color(color fill_color) {
    my_brush.my_fill_color = fill_color;
    my_renderer->update_state(renderer_dirty_flag::brush_color, static_cast<vec3f>(my_brush.my_fill_color));
}

void painter::set_brush_texture(texture fill_texture) {
    my_brush.my_fill_texture = fill_texture;
    my_renderer->update_state(renderer_dirty_flag::texture, static_cast<int64>(my_brush.my_fill_texture.id));
}

void painter::set_brush_opacity(float opacity) {
    my_brush.my_opacity = opacity;
    my_renderer->update_state(renderer_dirty_flag::brush_opacity, static_cast<float>(my_brush.my_opacity));
}
