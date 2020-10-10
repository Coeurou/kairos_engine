#pragma once

#include <opengl_renderer.h>

#include <nameof.hpp>

#include <core_application.h>
#include <contract.h>
#include <mathlib.h>
#include <shader.h>
#include <window.h>

static constexpr float no_texture = -1.f;
static constexpr size_t max_vertices = 8192;
static constexpr uint32 nb_points_line = 2;
static constexpr uint32 nb_points_rect = 4;
static constexpr uint32 nb_indices_rect = 6;
static constexpr const char* line_data = "line";
static constexpr const char* rectangle_data = "rectangle";

void opengl_renderer::setup() {
    // Define default shader for line and rectangle
    auto vertex_shader_source = R"(
    #version 130
    in vec4 position;
    in vec4 color;
    in vec2 texcoords;
    in float texindex;

    uniform mat4 projection_matrix;

    out vec4 vs_color;
    out vec2 vs_texcoords;
    flat out float vs_texindex;
    flat out vec2 vs_size;

    void main() {
        gl_Position = projection_matrix * vec4(position.xy, 0.0, 1.0);
        vs_color = color;
        vs_texcoords = texcoords;
        vs_texindex = texindex;
        vs_size = position.zw;
    })";

    auto fragment_shader_source_line = R"(
    #version 130
    in vec4 vs_color;

    out vec4 fs_color;
    void main() {
        fs_color =  vs_color;
    })";

    auto fragment_shader_source_rectangle = R"(
    #version 130
    in vec4 vs_color;
    in vec2 vs_texcoords;
    flat in float vs_texindex;
    flat in vec2 vs_size;

    uniform sampler2D textures[16];
    uniform float border_width;
    uniform vec4 border_color;

    out vec4 fs_color;
    void main() {
        int texindex = int(vs_texindex);

        float texture_pct = step(-0.1, vs_texindex);
        vec4 textured_color = texture_pct * texture(textures[texindex], vs_texcoords) + ((1.0 - texture_pct) * vec4(1.0));

        vec2 rectangle_border = vec2(border_width) / vs_size;
        vec2 bottom_left = step(rectangle_border, vs_texcoords);
        vec2 top_right = step(rectangle_border, vec2(1.0) - vs_texcoords);
        float pct = bottom_left.x * bottom_left.y * top_right.x * top_right.y;
        vec4 rectangle_color = ((1.0 - pct) * border_color) + (pct * vs_color);

        fs_color = rectangle_color * textured_color;
    })";

    shader default_vs;
    {
        default_vs.my_shader_name = "default_shader";
        default_vs.my_shader_type = GL_VERTEX_SHADER;
        default_vs.my_shader_source = vertex_shader_source;
        default_vs.setup();
    }
    shader default_line_fs;
    {
        default_line_fs.my_shader_name = "default_line_frag_shader";
        default_line_fs.my_shader_type = GL_FRAGMENT_SHADER;
        default_line_fs.my_shader_source = fragment_shader_source_line;
        default_line_fs.setup();
    }
    shader default_rect_fs;
    {
        default_rect_fs.my_shader_name = "default_rect_frag_shader";
        default_rect_fs.my_shader_type = GL_FRAGMENT_SHADER;
        default_rect_fs.my_shader_source = fragment_shader_source_rectangle;
        default_rect_fs.setup();
    }

    const auto setup_uniforms = [](uint32 program) {
        glUseProgram(program);
        const auto window_size = core_application::window()->size();
        mat4f orthographic_projection = ortho(rectf{{0.f, 0.f}, window_size});
        glUniformMatrix4fv(glGetUniformLocation(program, "projection_matrix"), 1,
            GL_FALSE, &orthographic_projection[0][0]);
        glUniform1f(glGetUniformLocation(program, "border_width"), 0.f);
        glUniform4f(glGetUniformLocation(program, "border_color"), 0.f, 0.f, 0.f, 0.f);
        for (auto i = 0; i < texture::our_limit; i++) {
            glUniform1i(glGetUniformLocation(program, fmt::format("textures[{}]", i).c_str()), i);
        }
        glUseProgram(0);
        check_gl_error();
    };

    // Define default vertices and indices buffers for line and rectangle
    const array<vertex> vertices(max_vertices);
    array<uint32> indices_line(max_vertices * nb_points_line);
    expects(indices_line.size() % nb_points_line == 0);
    for (size_t i = 0; i < indices_line.size(); i += nb_points_line) {
        indices_line[i] = 0;
        indices_line[i+1] = 1;
    }
    array<uint32> indices_rectangle(max_vertices * nb_indices_rect);
    expects(indices_rectangle.size() % nb_indices_rect == 0);
    size_t vertex_offset = 0;
    for (size_t i = 0; i < indices_rectangle.size(); i += nb_indices_rect) {
        indices_rectangle[i] = vertex_offset;
        indices_rectangle[i + 1] = vertex_offset + 1;
        indices_rectangle[i + 2] = vertex_offset + 2;
        indices_rectangle[i + 3] = vertex_offset;
        indices_rectangle[i + 4] = vertex_offset + 2;
        indices_rectangle[i + 5] = vertex_offset + 3;
        vertex_offset += nb_points_rect;
    }

    // Proper setup
    opengl_draw_data line_draw_data;
    {
        line_draw_data.setup(vertices, GL_DYNAMIC_DRAW, indices_line, {default_vs, default_line_fs},
                             line_data, GL_LINES);
        setup_uniforms(line_draw_data.my_program);

        my_draw_data.emplace(line_draw_data.my_program.my_name, std::move(line_draw_data));
    }

    opengl_draw_data rectangle_draw_data;
    {
        rectangle_draw_data.setup(vertices, GL_DYNAMIC_DRAW, indices_rectangle,
                                  {default_vs, default_rect_fs},
                                  rectangle_data, GL_TRIANGLES);
        setup_uniforms(rectangle_draw_data.my_program);

        my_draw_data.emplace(rectangle_draw_data.my_program.my_name, std::move(rectangle_draw_data));
    }
}

void opengl_renderer::cleanup() {
    for (auto& [identifier, rendering_params] : my_draw_data) {
        rendering_params.cleanup();
        check_gl_error();
    }
    my_draw_data.clear();
}

void opengl_renderer::update_state(renderer_dirty_flag flag, const variant& value) {
    switch (flag) {
    case renderer_dirty_flag::brush_color: {
        const auto brush_color = std::get_if<vec3f>(&value);
        expects(brush_color != nullptr, "Non-color value passed as argument for brush color");
        my_fill_color_value = color{ *brush_color, 1.f };
        break;
    }
    case renderer_dirty_flag::brush_opacity: {
        const auto brush_opacity = std::get_if<float>(&value);
        expects(brush_opacity != nullptr, "Non-float value passed as argument for brush opacity");
        my_brush_opacity_value = *brush_opacity;
        break;
    }
    case renderer_dirty_flag::line_width: {
        const auto line_width = std::get_if<float>(&value);
        expects(line_width != nullptr, "Non-float value passed as argument for line width");
        my_line_width_value = *line_width;
        for (auto& [_, draw_data] : my_draw_data) {
            glUseProgram(draw_data.my_program);
            glUniform1f(glGetUniformLocation(draw_data.my_program, "border_width"), my_line_width_value);
        }
        glUseProgram(0);
        break;
    }
    case renderer_dirty_flag::pen_color: {
        const auto pen_color = std::get_if<vec3f>(&value);
        expects(pen_color != nullptr, "Non-color value passed as argument for pen color");
        my_color_value = color{ *pen_color, 1.f };
        for (auto& [_, draw_data] : my_draw_data) {
            glUseProgram(draw_data.my_program);
            glUniform4fv(glGetUniformLocation(draw_data.my_program, "border_color"), 1, &my_color_value[0]);
        }
        break;
    }
    case renderer_dirty_flag::pen_opacity: {
        const auto pen_opacity = std::get_if<float>(&value);
        expects(pen_opacity != nullptr, "Non-float value passed as argument for pen opacity");
        my_pen_opacity_value = *pen_opacity;
        break;
    }
    case renderer_dirty_flag::texture: {
        const auto texture = std::get_if<int>(&value);
        expects(texture != nullptr, "Non-int value passed as argument for texture unit");
        my_fill_texture_value = static_cast<float>(*texture);
        break;
    }
    case renderer_dirty_flag::viewport: {
        const auto viewport = std::get_if<string>(&value);
        expects(viewport != nullptr, "Non-string value passed as argument for viewport");
        for (auto& [_, draw_data] : my_draw_data) {
            glUseProgram(draw_data.my_program);
            const rectf viewport_rect = from_string<rectf>(*viewport);
            mat4f orthographic_projection = ortho(viewport_rect);
            glUniformMatrix4fv(glGetUniformLocation(draw_data.my_program, "projection_matrix"), 1, GL_FALSE, &orthographic_projection[0][0]);
        }
        glUseProgram(0);
        break;
    }
    default: {
        ensures(false, fmt::format("Unimplemented rendering state, you shouldn't enter here: {}\n", NAMEOF_ENUM(flag)).c_str());
        break;
    }
    }
}

void opengl_renderer::render_lines(const program& program, uint32 indices_count,
                                   uint32 vertices_offset) const {
    expects(my_line_width_value > 0.f, "Cannot draw a line with width less than or equal to zero");
    float previous_line_width = 0.f;
    glGetFloatv(GL_LINE_WIDTH, &previous_line_width);

    glLineWidth(my_line_width_value);
    render(material{program, program.my_name}, indices_count, vertices_offset);
    glLineWidth(previous_line_width);
    check_gl_error();
}

void opengl_renderer::draw_line(const linef& line) {
    auto& geometry = my_draw_data[line_data];
    const uint32 vertex_offset = geometry.my_vertices_count;
    geometry.add_draw_data(to_vertices(line, my_fill_color_value, no_texture));
    check_gl_error();

    render_lines(geometry.my_program, nb_points_line, vertex_offset);
}

void opengl_renderer::draw_lines(const array<linef>& lines) {
    const size_t nb_points = lines.size() * nb_points_line;
    array<vertex> vertices{};
    vertices.reserve(nb_points);

    auto& geometry = my_draw_data[line_data];
    const uint32 vertex_offset = geometry.my_vertices_count;
    for (const auto& line : lines) {
        const auto line_vertices = to_vertices(line, my_fill_color_value, no_texture);
        vertices.insert(vertices.end(), line_vertices.begin(), line_vertices.end());
    }
    geometry.add_draw_data(vertices);
    check_gl_error();

    render_lines(geometry.my_program, nb_points, vertex_offset);
}

void opengl_renderer::draw_lines(const array<std::pair<linef, color>>& colored_lines) {
    const size_t nb_points = colored_lines.size() * nb_points_line;
    array<vertex> vertices{};
    vertices.reserve(nb_points);

    auto& geometry = my_draw_data[line_data];
    const uint32 vertex_offset = geometry.my_vertices_count;
    for (const auto& [line, color] : colored_lines) {
        const auto line_vertices = to_vertices(line, color, no_texture);
        vertices.insert(vertices.end(), line_vertices.begin(), line_vertices.end());
    }
    geometry.add_draw_data(vertices);
    check_gl_error();

    render_lines(geometry.my_program, nb_points, vertex_offset);
}

void opengl_renderer::draw_rect(const rectf& rect) {
    auto& geometry = my_draw_data[rectangle_data];
    const uint32 vertex_offset = geometry.my_vertices_count;
    geometry.add_draw_data(to_vertices(rect, my_fill_color_value, my_fill_texture_value));
    check_gl_error();

    render(material{geometry.my_program.my_gl_id, geometry.my_program.my_name}, nb_indices_rect,
           vertex_offset);
}

void opengl_renderer::draw_rects(const array<rectf>& rects) {
    array<vertex> vertices{};
    vertices.reserve(rects.size() * nb_points_rect);

    auto& geometry = my_draw_data[rectangle_data];
    const uint32 vertex_offset = geometry.my_vertices_count;
    for (const auto& rect : rects) {
        const auto rect_vertices = to_vertices(rect, my_fill_color_value, my_fill_texture_value);
        vertices.insert(vertices.end(), rect_vertices.begin(), rect_vertices.end());
    }
    geometry.add_draw_data(vertices);
    check_gl_error();

    render(material{geometry.my_program.my_gl_id, geometry.my_program.my_name}, rects.size() * nb_indices_rect,
           vertex_offset);
}

void opengl_renderer::draw_rects(const array<std::pair<rectf, color>>& colored_rects) {
    array<vertex> vertices{};
    vertices.reserve(colored_rects.size() * nb_points_rect);

    auto& geometry = my_draw_data[rectangle_data];
    const uint32 vertex_offset = geometry.my_vertices_count;
    for (const auto& [rect, color] : colored_rects) {
        const auto rect_vertices = to_vertices(rect, color, no_texture);
        vertices.insert(vertices.end(), rect_vertices.begin(), rect_vertices.end());
    }
    geometry.add_draw_data(vertices);
    check_gl_error();

    render(material{geometry.my_program.my_gl_id, geometry.my_program.my_name},
           colored_rects.size() * nb_indices_rect,
           vertex_offset);
}

void opengl_renderer::draw_rects(const array<std::pair<rectf, texture>>& textured_rects) {
    array<vertex> vertices{};
    vertices.reserve(textured_rects.size() * nb_points_rect);

    auto& geometry = my_draw_data[rectangle_data];
    const uint32 vertex_offset = geometry.my_vertices_count;
    for (const auto& [rect, texture] : textured_rects) {
        const auto rect_vertices =
            to_vertices(rect, color{1.f}, static_cast<float>(texture.my_index));
        vertices.insert(vertices.end(), rect_vertices.begin(), rect_vertices.end());
    }
    geometry.add_draw_data(vertices);
    check_gl_error();

    render(material{geometry.my_program.my_gl_id, geometry.my_program.my_name},
           textured_rects.size() * nb_indices_rect,
           vertex_offset);
}

void opengl_renderer::draw_sprite(const sprite& sprite) {
    auto& geometry = my_draw_data[rectangle_data];
    const uint32 vertex_offset = geometry.my_vertices_count;
    geometry.add_draw_data(
        to_vertices(sprite, sprite.my_color,
                    (sprite.my_texture != nullptr) ? sprite.my_texture->my_index : -1));
    check_gl_error();

    render(material{geometry.my_program.my_gl_id, geometry.my_program.my_name}, nb_indices_rect,
           vertex_offset);
}

void opengl_renderer::draw_sprites(const array<sprite>& sprites) {
    array<vertex> vertices{};
    vertices.reserve(sprites.size() * nb_points_rect);

    auto& geometry = my_draw_data[rectangle_data];
    const uint32 vertex_offset = geometry.my_vertices_count;
    for (const auto& sprite : sprites) {
        const auto sprite_vertices = to_vertices(sprite.my_bounds, sprite.my_color,
                                                 (sprite.my_texture != nullptr) ? sprite.my_texture->my_index : -1);
        vertices.insert(vertices.end(), sprite_vertices.begin(), sprite_vertices.end());
    }
    geometry.add_draw_data(vertices);
    check_gl_error();

    render(material{geometry.my_program.my_gl_id, geometry.my_program.my_name},
           sprites.size() * nb_indices_rect, vertex_offset);
}

void opengl_renderer::draw_ellipse(pointf center, float rx, float ry) {
}

void opengl_renderer::draw_ellipses(const array<rectf>& rects) {
    for (const auto& rect : rects) {
    }
}

void opengl_renderer::draw_ellipses(const array<std::pair<rectf, color>>& colored_ellipses) {
    for (const auto& [rect, color] : colored_ellipses) {
    }
}

void opengl_renderer::draw_ellipses(const array<std::pair<rectf, texture>>& textured_ellipses) {
    for (const auto& [rect, texture] : textured_ellipses) {
    }
}

void opengl_renderer::draw_text(pointf pos, string_view text) {
}

void opengl_renderer::render(material material, uint32 indices_count,
                             uint32 vertices_offset) const {
    expects(my_draw_data.find(material.my_name) != my_draw_data.end(), "Unknown material cannot be rendered without its parameters");

    const auto& draw_data = my_draw_data.at(material.my_name);
    draw_data.my_vao.bind();
    {
        glUseProgram(draw_data.my_program);
        glDrawElementsBaseVertex(draw_data.my_primitive, indices_count, GL_UNSIGNED_INT, nullptr,
                                 vertices_offset);
    }
    draw_data.my_vao.unbind();
    glUseProgram(0);
}
