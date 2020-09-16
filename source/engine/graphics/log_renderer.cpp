#pragma once

#include <log_renderer.h>

#include <nameof.hpp>

#include <application.h>
#include <brush.h>
#include <error.h>
#include <formattable.h>
#include <logger.h>
#include <pen.h>

void log_renderer::update_state(renderer_dirty_flag flag, const variant& value)
{
    string_view state_updated{};

    switch (flag) {
    case renderer_dirty_flag::brush_color:
        my_fill_color_value = to_string(&value);
        state_updated = my_fill_color_value;
        break;
    case renderer_dirty_flag::brush_opacity:
        my_brush_opacity_value = to_string(&value);
        state_updated = my_brush_opacity_value;
        break;
    case renderer_dirty_flag::line_width:
        my_line_width_value = to_string(&value);
        state_updated = my_line_width_value;
        break;
    case renderer_dirty_flag::pen_color:
        my_color_value = to_string(&value);
        state_updated = my_color_value;
        break;
    case renderer_dirty_flag::pen_opacity:
        my_pen_opacity_value = to_string(&value);
        state_updated = my_pen_opacity_value;
        break;
    case renderer_dirty_flag::texture:
        my_fill_texture_value = to_string(&value);
        state_updated = my_fill_texture_value;
        break;
    default:
        ASSERT_MSG(false, fmt::format("Unimplemented rendering state, you shouldn't enter here: {}", NAMEOF_ENUM(flag)).c_str());
        break;
    }

    log(LoggerName::GRAPHICS, "Flag {} has been updated with value {}", NAMEOF_ENUM(flag), state_updated.data());
}

void log_renderer::draw_line(const linef& line)
{
    log(LoggerName::GRAPHICS, "Draw {}, frame: {}", to_string(line), frame_number);
}

void log_renderer::draw_lines(const array<linef>& lines)
{
    string msg = fmt::format("{} lines drawn, frame: {}\n", lines.size(), frame_number);
    for (const auto& line : lines) {
        msg = fmt::format("{}- {}\n", msg, to_string(line));
    }
    log(LoggerName::GRAPHICS, "{}", msg);
}

void log_renderer::draw_lines(const array<std::pair<linef, color>>& colored_lines)
{
    string msg = fmt::format("{} colored lines drawn, frame: {}\n", colored_lines.size(), frame_number);
    for (const auto& [line, color] : colored_lines) {
        msg = fmt::format("{}- at {}, color: {}\n", msg, to_string(line), to_string(color));
    }
    log(LoggerName::GRAPHICS, "{}", msg);
}

void log_renderer::draw_rect(const rectf& rect)
{
    log(LoggerName::GRAPHICS, "Draw {}, frame: {}", to_string(rect), frame_number);
}

void log_renderer::draw_rects(const array<rectf>& rects)
{
    string msg = fmt::format("{} rectangles drawn, frame: {}\n", rects.size(), frame_number);
    for (const auto& rect : rects) {
        msg = fmt::format("{}- {}\n", msg, to_string(rect));
    }
    log(LoggerName::GRAPHICS, "{}", msg);
}

void log_renderer::draw_rects(const array<std::pair<rectf, color>>& colored_rects)
{
    string msg = fmt::format("{} colored rectangles drawn, frame: {}\n", colored_rects.size(), frame_number);
    for (const auto& [rect, color] : colored_rects) {
        msg = fmt::format("{}- {}, color: {}\n", msg, to_string(rect), to_string(color));
    }
    log(LoggerName::GRAPHICS, "{}", msg);
}

void log_renderer::draw_rects(const array<std::pair<rectf, texture>>& textured_rects)
{
    string msg = fmt::format("{} textured rectangles drawn, frame: {}\n", textured_rects.size(), frame_number);
    for (const auto& [rect, texture] : textured_rects) {
        msg = fmt::format("{}- {}, texture: {}\n", msg, to_string(rect), to_string(texture));
    }
    log(LoggerName::GRAPHICS, "{}", msg);
}

void log_renderer::draw_ellipse(pointf center, float rx, float ry)
{
    log(LoggerName::GRAPHICS, "Draw ellipse with center: {} radius x: {}, y: {}, frame: {}", to_string(center), rx, ry, frame_number);
}

void log_renderer::draw_ellipses(const array<rectf>& rects)
{
    string msg = fmt::format("{} ellipses drawn, frame: {}\n", rects.size(), frame_number);
    for (const auto& rect : rects) {
        msg = fmt::format("{}- contained in {}\n", msg, to_string(rect));
    }
    log(LoggerName::GRAPHICS, "{}", msg);
}

void log_renderer::draw_ellipses(const array<std::pair<rectf, color>>& colored_ellipses)
{
    string msg = fmt::format("{} colored rectangles drawn, frame: {}\n", colored_ellipses.size(), frame_number);
    for (const auto& [rect, color] : colored_ellipses) {
        msg = fmt::format("{}- contained in {}, color: {}\n", msg, to_string(rect), to_string(color));
    }
    log(LoggerName::GRAPHICS, "{}", msg);
}

void log_renderer::draw_ellipses(const array<std::pair<rectf, texture>>& textured_ellipses)
{
    string msg = fmt::format("{} textured ellipses drawn, frame: {}\n", textured_ellipses.size(), frame_number);
    for (const auto& [rect, texture] : textured_ellipses) {
        msg = fmt::format("{}- contained in {}, texture: {}\n", msg, to_string(rect), to_string(texture));
    }
    log(LoggerName::GRAPHICS, "{}", msg);
}

void log_renderer::draw_text(pointf pos, string_view text)
{
    log(LoggerName::GRAPHICS, "Draw text {} at position {}, frame {}", text.data(), to_string(pos), frame_number);
}
