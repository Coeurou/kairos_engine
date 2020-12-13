#pragma once

#include <graphics/log_renderer.h>

#include <nameof.hpp>

#include <graphics/brush.h>
#include <core/contract.h>
#include <core/formattable.h>
#include <core/logger.h>
#include <core/time.h>
#include <graphics/pen.h>

void log_renderer::setup(const sizef& /*window_size*/) {
    log(LoggerName::GRAPHICS, "Log renderer successfully setup\n");
}

void log_renderer::cleanup() {
    my_color_value = "";
    my_line_width_value = "";
    my_pen_opacity_value = "";
    my_fill_texture_value = "";
    my_fill_color_value = "";
    my_brush_opacity_value = "";
    log(LoggerName::GRAPHICS, "Log renderer successfully cleanup\n");
}

void log_renderer::update_state(renderer_dirty_flag flag, const variant& value) {
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
        ensures(false, fmt::format("Unimplemented rendering state, you shouldn't enter here: {}\n", NAMEOF_ENUM(flag)).c_str());
        break;
    }

    log(LoggerName::GRAPHICS, "Flag {} has been updated with value {}\n", NAMEOF_ENUM(flag), state_updated.data());
}

void log_renderer::draw_line(const linef& line) {
    log(LoggerName::GRAPHICS, "Draw {}, frame: {}\n", to_string(line), kairos::frame_number());
}

void log_renderer::draw_lines(const array<linef>& lines) {
    string msg = fmt::format("{} lines drawn, frame: {}\n", lines.size(), kairos::frame_number());
    for (const auto& line : lines) {
        msg = fmt::format("{}- {}\n", msg, to_string(line));
    }
    log(LoggerName::GRAPHICS, "{}\n", msg);
}

void log_renderer::draw_lines(const array<std::pair<linef, color>>& colored_lines) {
    string msg = fmt::format("{} colored lines drawn, frame: {}\n", colored_lines.size(),
                             kairos::frame_number());
    for (const auto& [line, color] : colored_lines) {
        msg = fmt::format("{}- at {}, color: {}\n", msg, to_string(line), to_string(color));
    }
    log(LoggerName::GRAPHICS, "{}\n", msg);
}

void log_renderer::draw_rect(const rectf& rect) {
    log(LoggerName::GRAPHICS, "Draw {}, frame: {}\n", to_string(rect), kairos::frame_number());
}

void log_renderer::draw_rects(const array<rectf>& rects) {
    string msg =
        fmt::format("{} rectangles drawn, frame: {}\n", rects.size(), kairos::frame_number());
    for (const auto& rect : rects) {
        msg = fmt::format("{}- {}\n", msg, to_string(rect));
    }
    log(LoggerName::GRAPHICS, "{}\n", msg);
}

void log_renderer::draw_rects(const array<std::pair<rectf, color>>& colored_rects) {
    string msg = fmt::format("{} colored rectangles drawn, frame: {}\n", colored_rects.size(),
                             kairos::frame_number());
    for (const auto& [rect, color] : colored_rects) {
        msg = fmt::format("{}- {}, color: {}\n", msg, to_string(rect), to_string(color));
    }
    log(LoggerName::GRAPHICS, "{}\n", msg);
}

void log_renderer::draw_rects(const array<std::pair<rectf, texture>>& textured_rects) {
    string msg = fmt::format("{} textured rectangles drawn, frame: {}\n", textured_rects.size(),
                             kairos::frame_number());
    for (const auto& [rect, texture] : textured_rects) {
        msg = fmt::format("{}- {}, texture: {}\n", msg, to_string(rect), to_string(texture));
    }
    log(LoggerName::GRAPHICS, "{}\n", msg);
}

void log_renderer::draw_sprite(const sprite& sprite) {
    log(LoggerName::GRAPHICS, "Draw {}, frame: {}\n", to_string(sprite), kairos::frame_number());
}

void log_renderer::draw_sprites(const array<sprite>& sprites) {
    string msg =
        fmt::format("{} sprites drawn, frame: {}\n", sprites.size(), kairos::frame_number());
    for (const auto& sprite : sprites) {
        msg = fmt::format("{} - {}\n", msg, to_string(sprite));
    }
    log(LoggerName::GRAPHICS, "{}\n", msg);
}

void log_renderer::draw_ellipse(const pointf& center, float rx, float ry) {
    log(LoggerName::GRAPHICS, "Draw ellipse with center: {} radius x: {}, y: {}, frame: {}\n",
        to_string(center), rx, ry, kairos::frame_number());
}

void log_renderer::draw_ellipses(const array<rectf>& rects) {
    string msg =
        fmt::format("{} ellipses drawn, frame: {}\n", rects.size(), kairos::frame_number());
    for (const auto& rect : rects) {
        msg = fmt::format("{}- contained in {}\n", msg, to_string(rect));
    }
    log(LoggerName::GRAPHICS, "{}\n", msg);
}

void log_renderer::draw_ellipses(const array<std::pair<rectf, color>>& colored_ellipses) {
    string msg = fmt::format("{} colored rectangles drawn, frame: {}\n", colored_ellipses.size(),
                             kairos::frame_number());
    for (const auto& [rect, color] : colored_ellipses) {
        msg = fmt::format("{}- contained in {}, color: {}\n", msg, to_string(rect), to_string(color));
    }
    log(LoggerName::GRAPHICS, "{}\n", msg);
}

void log_renderer::draw_ellipses(const array<std::pair<rectf, texture>>& textured_ellipses) {
    string msg = fmt::format("{} textured ellipses drawn, frame: {}\n", textured_ellipses.size(),
                             kairos::frame_number());
    for (const auto& [rect, texture] : textured_ellipses) {
        msg = fmt::format("{}- contained in {}, texture: {}\n", msg, to_string(rect), to_string(texture));
    }
    log(LoggerName::GRAPHICS, "{}\n", msg);
}

void log_renderer::draw_text(const pointf& pos, string_view text) {
    log(LoggerName::GRAPHICS, "Draw text {} at position {}, frame {}\n", text.data(),
        to_string(pos), kairos::frame_number());
}
