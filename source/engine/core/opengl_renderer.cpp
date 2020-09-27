#pragma once

#include <opengl_renderer.h>

#include <nameof.hpp>

#include <core_application.h>
#include <contract.h>
#include <mathlib.h>
#include <shader.h>
#include <window.h>

static constexpr float no_texture = -1.f;
static constexpr uint32 nb_points_line = 2;
static constexpr uint32 nb_points_rect = 4;
static constexpr const char* line_data = "line";
static constexpr const char* rectangle_data = "rectangle";

void opengl_renderer::setup() {
    auto vertex_shader_source = R"(
    #version 130
    in vec3 position;
    in vec4 color;
    in vec2 texcoords;
    in float texindex;

    uniform mat4 projection_matrix;

    out vec4 vs_color;
    out vec2 vs_texcoords;
    flat out float vs_texindex;
    void main() {
        gl_Position = projection_matrix * vec4(position, 1.0f);
        vs_color = color;
        vs_texcoords = texcoords;
        vs_texindex = texindex;
    })";

    auto fragment_shader_source = R"(
    #version 130
    in vec4 vs_color;
    in vec2 vs_texcoords;
    flat in float vs_texindex;

    uniform sampler2D textures[16];

    out vec4 fs_color;
    void main() {
        int texindex = int(vs_texindex);
        if (texindex < 0 || texindex >= 16) {
            fs_color = vs_color;
        }
        else {
            fs_color = texture(textures[texindex], vs_texcoords);
        }
    })";

    shader default_vs;
    {
        default_vs.my_shader_name = "default_shader";
        default_vs.my_shader_type = GL_VERTEX_SHADER;
        default_vs.my_shader_source = vertex_shader_source;
        default_vs.setup();
    }
    shader default_fs;
    {
        default_fs.my_shader_name = "default_shader";
        default_fs.my_shader_type = GL_FRAGMENT_SHADER;
        default_fs.my_shader_source = fragment_shader_source;
        default_fs.setup();
    }

    const auto setup_uniforms = [](uint32 program) {
        glUseProgram(program);
        mat4f orthographic_projection =
            ortho(rectf{{0.f, 0.f}, core_application::window()->size()});
        glUniformMatrix4fv(glGetUniformLocation(program, "projection_matrix"), 1,
            GL_FALSE, &orthographic_projection[0][0]);
        for (auto i = 0; i < texture::our_limit; i++) {
            glUniform1i(glGetUniformLocation(program, fmt::format("textures[{}]", i).c_str()), i);
        }
        glUseProgram(0);
        check_gl_error();
    };

    const array<vertex> vertices(size_t(8192));
    const array<uint32> indices(size_t(32768));
    opengl_draw_data line_draw_data;
    {
        line_draw_data.setup(vertices, GL_DYNAMIC_DRAW, indices, {default_vs, default_fs},
                             line_data, GL_LINES);
        setup_uniforms(line_draw_data.my_program);

        my_draw_data.emplace(line_draw_data.my_program.my_name, std::move(line_draw_data));
    }

    opengl_draw_data rectangle_draw_data;
    {
        rectangle_draw_data.setup(vertices, GL_DYNAMIC_DRAW, indices, {default_vs, default_fs},
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
        break;
    }
    case renderer_dirty_flag::pen_color: {
        const auto pen_color = std::get_if<vec3f>(&value);
        expects(pen_color != nullptr, "Non-color value passed as argument for pen color");
        my_color_value = color{ *pen_color, 1.f };
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
        expects(texture != nullptr, "Non-string value passed as argument for texture");
        my_fill_texture_value = static_cast<float>(*texture);
        break;
    }
    case renderer_dirty_flag::viewport: {
        const auto viewport = std::get_if<string>(&value);
        expects(viewport != nullptr, "Non-string value passed as argument for texture");
        for (auto& [identifier, draw_data] : my_draw_data) {
            glUseProgram(draw_data.my_program);
            mat4f orthographic_projection = ortho(from_string<rectf>(*viewport));
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

void opengl_renderer::draw_line(const linef& line) {
    auto& geometry = my_draw_data[line_data];
    const auto indices_range = geometry.add_draw_data(to_vertices(line, my_fill_color_value, no_texture),
        to_indices<linef>(geometry.my_vertices_count));
    check_gl_error();

    render(material{ geometry.my_program.my_gl_id, geometry.my_program.my_name }, indices_range.first, indices_range.second);
}

void opengl_renderer::draw_lines(const array<linef>& lines) {
    const size_t nb_points = lines.size() * nb_points_line;
    array<vertex> vertices{};
    vertices.reserve(nb_points);
    array<uint32> indices{};
    indices.reserve(nb_points);

    auto& geometry = my_draw_data[line_data];
    uint32 current_index = 0;
    for (const auto& line : lines) {
        const auto line_vertices = to_vertices(line, my_fill_color_value, no_texture);
        vertices.insert(vertices.end(), line_vertices.begin(), line_vertices.end());

        const auto line_indices = to_indices<linef>(current_index + geometry.my_vertices_count);
        indices.insert(indices.end(), line_indices.begin(), line_indices.end());
        current_index += nb_points_line;
    }
    const auto indices_range = geometry.add_draw_data(vertices, indices);
    check_gl_error();

    render(material{ geometry.my_program.my_gl_id, geometry.my_program.my_name }, indices_range.first, indices_range.second);
}

void opengl_renderer::draw_lines(const array<std::pair<linef, color>>& colored_lines) {
    const size_t nb_points = colored_lines.size() * nb_points_line;
    array<vertex> vertices{};
    vertices.reserve(nb_points);
    array<uint32> indices{};
    indices.reserve(nb_points);

    auto& geometry = my_draw_data[line_data];
    uint32 current_index = 0;
    for (const auto& [line, color] : colored_lines) {
        const auto line_vertices = to_vertices(line, color, no_texture);
        vertices.insert(vertices.end(), line_vertices.begin(), line_vertices.end());

        const auto line_indices = to_indices<linef>(current_index + geometry.my_vertices_count);
        indices.insert(indices.end(), line_indices.begin(), line_indices.end());
        current_index += nb_points_line;
    }
    const auto indices_range = geometry.add_draw_data(vertices, indices);
    check_gl_error();

    render(material{geometry.my_program.my_gl_id, geometry.my_program.my_name}, indices_range.first,
           indices_range.second);
}

void opengl_renderer::draw_rect(const rectf& rect) {
    auto& geometry = my_draw_data[rectangle_data];
    const auto indices_range = geometry.add_draw_data(
        to_vertices(rect, my_fill_color_value, my_fill_texture_value),
        to_indices<rectf>(geometry.my_vertices_count));
    check_gl_error();

    render(material{geometry.my_program.my_gl_id, geometry.my_program.my_name}, indices_range.first,
           indices_range.second);
}

void opengl_renderer::draw_rects(const array<rectf>& rects) {
    const size_t nb_points = rects.size() * nb_points_rect;
    array<vertex> vertices{};
    vertices.reserve(nb_points);
    array<uint32> indices{};
    indices.reserve(nb_points);

    auto& geometry = my_draw_data[rectangle_data];
    uint32 current_index = 0;
    for (const auto& rect : rects) {
        const auto rect_vertices = to_vertices(rect, my_fill_color_value, my_fill_texture_value);
        vertices.insert(vertices.end(), rect_vertices.begin(), rect_vertices.end());

        const auto rect_indices = to_indices<rectf>(current_index + geometry.my_vertices_count);
        indices.insert(indices.end(), rect_indices.begin(), rect_indices.end());
        current_index += nb_points_rect;
    }
    const auto indices_range = geometry.add_draw_data(vertices, indices);
    check_gl_error();

    render(material{geometry.my_program.my_gl_id, geometry.my_program.my_name}, indices_range.first,
           indices_range.second);
}

void opengl_renderer::draw_rects(const array<std::pair<rectf, color>>& colored_rects) {
    const size_t nb_points = colored_rects.size() * nb_points_rect;
    array<vertex> vertices{};
    vertices.reserve(nb_points);
    array<uint32> indices{};
    indices.reserve(nb_points);

    auto& geometry = my_draw_data[rectangle_data];
    uint32 current_index = 0;
    for (const auto& [rect, color] : colored_rects) {
        const auto rect_vertices = to_vertices(rect, color, no_texture);
        vertices.insert(vertices.end(), rect_vertices.begin(), rect_vertices.end());

        const auto rect_indices = to_indices<rectf>(current_index + geometry.my_vertices_count);
        indices.insert(indices.end(), rect_indices.begin(), rect_indices.end());
        current_index += nb_points_rect;
    }
    const auto indices_range = geometry.add_draw_data(vertices, indices);
    check_gl_error();

    render(material{geometry.my_program.my_gl_id, geometry.my_program.my_name}, indices_range.first,
           indices_range.second);
}

void opengl_renderer::draw_rects(const array<std::pair<rectf, texture>>& textured_rects) {
    const size_t nb_points = textured_rects.size() * nb_points_rect;
    array<vertex> vertices{};
    vertices.reserve(nb_points);
    array<uint32> indices{};
    indices.reserve(nb_points);

    auto& geometry = my_draw_data[rectangle_data];
    uint32 current_index = 0;
    for (const auto& [rect, texture] : textured_rects) {
        float texture_index = static_cast<float>(texture.my_index);
        const auto rect_vertices = to_vertices(rect, my_fill_color_value, texture_index);
        vertices.insert(vertices.end(), rect_vertices.begin(), rect_vertices.end());

        const auto rect_indices = to_indices<rectf>(current_index + geometry.my_vertices_count);
        indices.insert(indices.end(), rect_indices.begin(), rect_indices.end());
        current_index += nb_points_rect;
    }
    const auto indices_range = geometry.add_draw_data(vertices, indices);
    check_gl_error();

    render(material{geometry.my_program.my_gl_id, geometry.my_program.my_name}, indices_range.first,
           indices_range.second);
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

void opengl_renderer::render(material material, uint32 start_idx, uint32 end_idx) const {
    expects(my_draw_data.find(material.my_name) != my_draw_data.end(), "Unknown material cannot be rendered without its parameters");

    const auto& draw_data = my_draw_data.at(material.my_name);
    draw_data.my_vao.bind();
    {
        glUseProgram(draw_data.my_program);

        float previous_line_width = 0.f;
        glGetFloatv(GL_LINE_WIDTH, &previous_line_width);
        glLineWidth(my_line_width_value);

        glDrawRangeElements(draw_data.my_primitive, start_idx, end_idx, draw_data.my_indices_count, GL_UNSIGNED_INT, nullptr);

        glLineWidth(previous_line_width);
    }
    draw_data.my_vao.unbind();
    glUseProgram(0);
}
