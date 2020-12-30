#pragma once

#include <graphics/brush.h>
#include <graphics/pen.h>
#include <graphics/opengl/opengl_draw_data.h>
#include <math/rect.h>

namespace kairos {

struct material;
struct program;
class sprite;

class opengl_renderer {
  public:
    opengl_renderer(const sizef& canvas_size);
    opengl_renderer(pen pen, brush brush, const sizef& canvas_size);
    ~opengl_renderer();

    opengl_renderer(const opengl_renderer&) = delete;
    opengl_renderer& operator=(const opengl_renderer&) = delete;
    opengl_renderer(opengl_renderer&&) = default;
    opengl_renderer& operator=(opengl_renderer&&) noexcept = default;

    void setup(const sizef& canvas_size);
    void cleanup();

    // indices_count is the number of indices in the IBO to consider &
    // vertices_offset is the offset applied to indices for subgeometry.
    // it is useful for rendering only one shape at the time
    void render(const material& material, uint32 indices_count, uint32 vertices_offset) const;
    void render_lines(const program& program, uint32 indices_count, uint32 vertices_offset) const;

    const opengl_draw_data& get_draw_data(uint32 id) const;

    template <class Vertices> void add_draw_data(uint32 id, Vertices&& vertices) {
        my_draw_data[id].add_draw_data(std::forward<Vertices>(vertices));
    }

    void clear_draw_data();

    const pen& get_pen() const;
    void set_pen(pen pen);
    const brush& get_brush() const;
    void set_brush(brush brush);
    rectf canvas() const;

  private:
    pen my_pen;
    brush my_brush;
    dictionary<uint32, opengl_draw_data> my_draw_data;
    rectf my_canvas{};
};

/** Non-member functions */

void clear(opengl_renderer& renderer, const color& color);

void draw(opengl_renderer& renderer, const sprite& sprite);
void draw(opengl_renderer& renderer, const array<sprite>& sprites);

void draw(opengl_renderer& renderer, const rectf& rect);
void draw(opengl_renderer& renderer, const array<rectf>& rects);

void draw(opengl_renderer& renderer, const linef& line);
void draw(opengl_renderer& renderer, const array<linef>& lines);

// the way we use to_vertices define how we render a shape, it is the main
// difference between each version of draw_ function
template <typename T> auto to_vertices(const T& geometry, const color& color, float texture_index) {
    if constexpr (std::is_same_v<std::decay_t<T>, linef>) {
        return static_array<vertex, 2>{vertex{vec4f(geometry.first, 0.f, 1.f), color},
                                       vertex{vec4f(geometry.second, 0.f, 1.f), color}};
    } else if constexpr (std::is_same_v<std::decay_t<T>, rectf>) {
        auto rect_size = size(geometry);
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
    } else if constexpr (std::is_same_v<std::decay_t<T>, sprite>) {
        const rectf& bounds = geometry.my_bounds;
        const auto rect_size = size(bounds);
        float angle = to_radians(geometry.my_rotation);
        const pointf rotation_center = bounds.my_top_left + geometry.my_origin;

        const auto rotate_point = [](const pointf& p, float angle, const pointf& rotation_center) {
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

        const vec2i texture_size = size(geometry.my_texture);
        const sizef spritesheet_size = sizef{float(texture_size.x), float(texture_size.y)};
        const sizef texture_rect = size(geometry.my_texture_rect);
        // sprite texture rect is r(tx, ty, bx, by) => min uv x = tx / spritesheet.size.x, min
        // uv y = ty / spritesheet.size.y

        // don't like this name it must express the fact that we select a section from a
        // spritesheet to deduce uv
        const rectf uv_texture_selection = {
            {geometry.my_texture_rect.left() / float(texture_size.x),
             geometry.my_texture_rect.top() / float(texture_size.y)},
            {geometry.my_texture_rect.right() / float(texture_size.x),
             geometry.my_texture_rect.bottom() / float(texture_size.y)}};

        return static_array<vertex, 4>{
            vertex{vec4f(rotated_geometry[0], rect_size.x, rect_size.y), color,
                   uv_texture_selection.my_bottom_right, texture_index},
            vertex{vec4f(rotated_geometry[1], rect_size.x, rect_size.y),
                   color,
                   {uv_texture_selection.right(), uv_texture_selection.top()},
                   texture_index},
            vertex{vec4f(rotated_geometry[2], rect_size.x, rect_size.y), color,
                   uv_texture_selection.my_top_left, texture_index},
            vertex{vec4f(rotated_geometry[3], rect_size.x, rect_size.y),
                   color,
                   {uv_texture_selection.left(), uv_texture_selection.bottom()},
                   texture_index}};
    } else {
        static_assert(false, "Geometry type not supported");
    }
}

} // namespace kairos