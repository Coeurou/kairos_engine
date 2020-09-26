#pragma once

#include <opengl_renderer.h>

#include <nameof.hpp>

#include <application.h>
#include <brush.h>
#include <error.h>
#include <formattable.h>
#include <logger.h>
#include <pen.h>
#include <vao.h>
#include <program.h>
#include <shader.h>
#include <transform.h>
#include <mathlib.h>

void opengl_renderer::setup() {
    auto vertex_shader_source = R"(
    #version 330
    in vec3 position;
    in vec4 color;
    in vec2 texcoords;
    in int texindex;

    uniform mat4 projection_matrix;

    out vec4 vs_color;
    out vec2 vs_texcoords;
    flat out int vs_texindex;
    void main() {
        gl_Position = projection_matrix * vec4(position, 1.0f);
        vs_color = color;
        vs_texcoords = texcoords;
        vs_texindex = texindex;
    })";

    auto fragment_shader_source = R"(
    #version 330
    in vec4 vs_color;
    in vec2 vs_texcoords;
    flat in int vs_texindex;

    uniform sampler2D textures[16];

    out vec4 fs_color;
    void main() {
        if (vs_texindex < 0) {
            fs_color = vs_color;
        }
        else {
            fs_color = texture(textures[vs_texindex], vs_texcoords);
        }
    })";

    shader line_vs;
    {
        line_vs.my_shader_name = "default_shader";
        line_vs.my_shader_type = GL_VERTEX_SHADER;
        line_vs.my_shader_source = vertex_shader_source;
        line_vs.setup();
    }
    shader line_fs;
    {
        line_fs.my_shader_name = "default_shader";
        line_fs.my_shader_type = GL_FRAGMENT_SHADER;
        line_fs.my_shader_source = fragment_shader_source;
        line_fs.setup();
    }

    opengl_draw_data static_line_draw_data;
    {
        static_line_draw_data.setup(array<vertex>(size_t(8192)), GL_STATIC_DRAW, array<uint32>(size_t(32768)), { line_vs, line_fs }, "static_line", GL_LINES);

        glUseProgram(static_line_draw_data.my_program);
        mat4f orthographic_projection = ortho(rectf{ { 0.f, 0.f }, core_application::window()->size() });
        glUniformMatrix4fv(glGetUniformLocation(static_line_draw_data.my_program, "projection_matrix"), 1, GL_FALSE, &orthographic_projection[0][0]);
        glUseProgram(0);
        check_gl_error();

        my_draw_data.emplace(static_line_draw_data.my_program.my_name, std::move(static_line_draw_data));
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
        const auto texture = std::get_if<string>(&value);
        expects(texture != nullptr, "Non-string value passed as argument for texture");
        my_fill_texture_value = *texture;
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
        ASSERT_MSG(false, fmt::format("Unimplemented rendering state, you shouldn't enter here: {}\n", NAMEOF_ENUM(flag)).c_str());
        break;
    }
    }
}

void opengl_renderer::draw_line(const linef& line) {
    auto& geometry = my_draw_data["static_line"];
    const auto indices_range = geometry.add_draw_data(
        static_array<vertex, 2>{ vertex{ vec3f(line.first, 0.f), my_color_value }, vertex{ vec3f(line.second, 0.f), my_color_value } },
        static_array<uint32, 2> { 0 + geometry.my_indices_count, 1 + geometry.my_indices_count });
    check_gl_error();

    render(material{ geometry.my_program.my_gl_id, geometry.my_program.my_name }, indices_range.first, indices_range.second);
}

void opengl_renderer::draw_lines(const array<linef>& lines) {
    const size_t nb_points = lines.size() * 2;
    array<vertex> vertices{};
    vertices.reserve(nb_points);
    array<uint32> indices{};
    indices.reserve(nb_points);

    auto& geometry = my_draw_data["static_line"];
    uint32 current_index = 0;
    for (const auto& line : lines) {
        vertices.emplace_back(vertex{ vec3f(line.first, 0.f), my_color_value });
        vertices.emplace_back(vertex{ vec3f(line.second, 0.f), my_color_value });
        indices.emplace_back(current_index + geometry.my_indices_count);
        indices.emplace_back(++current_index + geometry.my_indices_count);
        ++current_index;
    }
    const auto indices_range = geometry.add_draw_data(vertices, indices);
    check_gl_error();

    render(material{ geometry.my_program.my_gl_id, geometry.my_program.my_name }, indices_range.first, indices_range.second);
}

void opengl_renderer::draw_lines(const array<std::pair<linef, color>>& colored_lines) {
    for (const auto& [line, color] : colored_lines) {
    }
}

void opengl_renderer::draw_rect(const rectf& rect) {
}

void opengl_renderer::draw_rects(const array<rectf>& rects) {
    for (const auto& rect : rects) {
    }
}

void opengl_renderer::draw_rects(const array<std::pair<rectf, color>>& colored_rects) {
    for (const auto& [rect, color] : colored_rects) {
    }
}

void opengl_renderer::draw_rects(const array<std::pair<rectf, texture>>& textured_rects) {
    for (const auto& [rect, texture] : textured_rects) {
    }
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
