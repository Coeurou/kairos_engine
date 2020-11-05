#pragma once

#include <graphics/abstract_2d_renderer.h>
#include <graphics/material.h>
#include <math/mathlib.h>
#include <graphics/opengl/opengl_draw_data.h>

class opengl_renderer : public abstract_renderer {
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

    void draw_ellipse(const pointf& center, float rx, float ry) override;

    void draw_ellipses(const array<rectf>& rects) override;
    void draw_ellipses(const array<std::pair<rectf, color>>& colored_ellipses) override;
    void draw_ellipses(const array<std::pair<rectf, texture>>& textured_ellipses) override;

    void draw_text(const pointf& pos, string_view text) override;

    // indices_count is the number of indices in the IBO to consider & 
    // vertices_offset is the offset applied to indices for subgeometry.
    // it is useful for rendering only one shape at the time
    void render(material material, uint32 indices_count, uint32 vertices_offset) const;

private:
    float my_line_width_value{ 0.f };
    float my_pen_opacity_value{ 0.f };
    float my_brush_opacity_value{ 0.f };
    color my_fill_color_value{ 0.f };
    color my_color_value{ 0.f };
    float my_fill_texture_value{ -1.f };

    dictionary<string, opengl_draw_data> my_draw_data;

    void render_lines(const program& program, uint32 indices_count, uint32 vertices_offset) const;

    // the way we use to_vertices define how we render a shape, it is the main
    // difference between each version of draw_ function
    template <typename T>
    auto to_vertices(const T& geometry, const color& color, float texture_index) {
        if constexpr (std::is_same_v<std::decay_t<T>, linef>) {
            return static_array<vertex, 2>{vertex{vec4f(geometry.first, 0.f, 1.f), color},
                                           vertex{vec4f(geometry.second, 0.f, 1.f), color}};
        }
        else if constexpr (std::is_same_v<std::decay_t<T>, rectf>) {
            auto rect_size = geometry.size();
            return static_array<vertex, 4>{
                vertex{vec4f(geometry.right(), geometry.top(), rect_size.x, rect_size.y),
                       color,
                       {1.f, 1.f},
                       texture_index},
                vertex{vec4f(geometry.my_bottom_right, rect_size.x, rect_size.y),
                       color,
                       {1.f, 0.f},
                       texture_index},
                vertex{vec4f(geometry.left(), geometry.bottom(), rect_size.x, rect_size.y),
                       color,
                       {0.f, 0.f},
                       texture_index},
                vertex{vec4f(geometry.my_top_left, rect_size.x, rect_size.y),
                       color,
                       {0.f, 1.f},
                       texture_index}};
        }
        else if constexpr (std::is_same_v<std::decay_t<T>, sprite>) {
            const rectf& bounds = geometry.my_bounds;
            const auto rect_size = bounds.size();
            float angle = to_radians(geometry.my_rotation);
            const pointf rotation_center = bounds.my_top_left + geometry.my_origin;

            const auto rotate_point = [](const pointf& p, float angle,
                                         const pointf& rotation_center) {
                return pointf{rotation_center.x + (p.x - rotation_center.x) * cos(angle) -
                                  (p.y - rotation_center.y) * sin(angle),
                              rotation_center.y + (p.x - rotation_center.x) * sin(angle) +
                                  (p.y - rotation_center.y) * cos(angle)};
            };
            const static_array<pointf, 4> rotated_geometry = {
                rotate_point(pointf(bounds.right(), bounds.top()), angle, rotation_center),
                rotate_point(bounds.my_bottom_right, angle, rotation_center),
                rotate_point(pointf(bounds.left(), bounds.bottom()), angle, rotation_center),
                rotate_point(bounds.my_top_left, angle, rotation_center)};

            const sizef spritesheet_size =
                sizef{float(geometry.my_texture->my_size.x), float(geometry.my_texture->my_size.y)};
            const sizef texture_rect = geometry.my_texture_rect.size();
            // sprite texture rect is r(tx, ty, bx, by) => min uv x = tx / spritesheet.size.x, min uv y = ty / spritesheet.size.y

            // don't like this name it must express the fact that we select a section from a
            // spritesheet to deduce uv
            const rectf uv_texture_selection = {
                {geometry.my_texture_rect.left() / float(geometry.my_texture->my_size.x),
                 geometry.my_texture_rect.top() / float(geometry.my_texture->my_size.y)},
                {geometry.my_texture_rect.right() / float(geometry.my_texture->my_size.x),
                 geometry.my_texture_rect.bottom() / float(geometry.my_texture->my_size.y)}};

            return static_array<vertex, 4>{
                vertex{vec4f(rotated_geometry[0], rect_size.x, rect_size.y),
                       color,
                       uv_texture_selection.my_bottom_right,
                       texture_index},
                vertex{vec4f(rotated_geometry[1], rect_size.x, rect_size.y),
                       color,
                       {uv_texture_selection.right(), uv_texture_selection.top()},
                       texture_index},
                vertex{vec4f(rotated_geometry[2], rect_size.x, rect_size.y),
                       color,
                       uv_texture_selection.my_top_left,
                       texture_index},
                vertex{vec4f(rotated_geometry[3], rect_size.x, rect_size.y),
                       color,
                       {uv_texture_selection.left(), uv_texture_selection.bottom()},
                       texture_index}};
        }
        else {
            static_assert(false, "Geometry type not supported");
        }
    }

    template <typename T> auto to_indices(uint32 offset) {
        if constexpr (std::is_same_v<std::decay_t<T>, linef>) {
            return static_array<uint32, 2>{0 + offset, 1 + offset};
        } else if constexpr (std::is_same_v<std::decay_t<T>, rectf>) {
            return static_array<uint32, 6>{0 + offset, 1 + offset, 2 + offset,
                                           0 + offset, 2 + offset, 3 + offset};
        } else {
            static_assert(false, "Geometry type not supported");
        }
    }
};

inline void check_gl_error() {
    auto error = glGetError();
    ensures(error == GL_NO_ERROR, fmt::format("Last GL error: {}", error));
}