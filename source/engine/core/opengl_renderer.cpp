#pragma once

#include <core/opengl_renderer.h>

#include <core/contract.h>
#include <core/hash.h>
#include <graphics/material.h>
#include <graphics/opengl/shader.h>
#include <graphics/sprite.h>
#include <graphics/vertex.h>
#include <math/mathlib.h>
#include <graphics/opengl/opengl_context.h>

namespace kairos {

static constexpr float no_texture = -1.f;
static constexpr size_t max_vertices = 8192;
static constexpr uint32 nb_points_line = 2;
static constexpr uint32 nb_points_rect = 4;
static constexpr uint32 nb_indices_rect = 6;
static constexpr const char* line_data_name = "line";
static constexpr const char* rectangle_data_name = "rectangle";
static constexpr uint32 line_data = hash(line_data_name);
static constexpr uint32 rectangle_data = hash(rectangle_data_name);

opengl_renderer::opengl_renderer(const sizef& canvas_size) { setup(canvas_size); }

opengl_renderer::opengl_renderer(pen pen, brush brush, const sizef& canvas_size)
    : my_pen(std::move(pen)), my_brush(std::move(brush)) {
    setup(canvas_size);
}

opengl_renderer::~opengl_renderer() { cleanup(); }

void opengl_renderer::setup(const sizef& canvas_size) {
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

    const auto setup_uniforms = [&canvas_size](uint32 program) {
        glUseProgram(program);
        mat4f orthographic_projection = ortho(rectf{{0.f, 0.f}, canvas_size});
        glUniformMatrix4fv(glGetUniformLocation(program, "projection_matrix"), 1, GL_FALSE,
                           &orthographic_projection[0][0]);
        glUniform1f(glGetUniformLocation(program, "border_width"), 0.f);
        glUniform4f(glGetUniformLocation(program, "border_color"), 0.f, 0.f, 0.f, 0.f);
        for (auto i = 0; i < texture::our_limit; i++) {
            glUniform1i(glGetUniformLocation(program, fmt::format("textures[{}]", i).c_str()), i);
        }
        glUseProgram(0);
        check_gl_error(__FUNCTION__);
    };

    // Define default vertices and indices buffers for line and rectangle
    const array<vertex> vertices(max_vertices);
    array<uint32> indices_line(max_vertices * nb_points_line);
    expects(indices_line.size() % nb_points_line == 0);
    uint32 vertex_offset = 0;
    for (size_t i = 0; i < indices_line.size(); i += nb_points_line) {
        indices_line[i] = vertex_offset + 0;
        indices_line[i + 1] = vertex_offset + 1;
        vertex_offset += nb_points_line;
    }
    array<uint32> indices_rectangle(max_vertices * nb_indices_rect);
    expects(indices_rectangle.size() % nb_indices_rect == 0);
    vertex_offset = 0;
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
                             line_data_name, GL_LINES);
        setup_uniforms(line_draw_data.my_program);

        my_draw_data.emplace(hash(line_draw_data.my_program.my_name), std::move(line_draw_data));
    }

    opengl_draw_data rectangle_draw_data;
    {
        rectangle_draw_data.setup(vertices, GL_DYNAMIC_DRAW, indices_rectangle,
                                  {default_vs, default_rect_fs}, rectangle_data_name, GL_TRIANGLES);
        setup_uniforms(rectangle_draw_data.my_program);

        my_draw_data.emplace(hash(rectangle_draw_data.my_program.my_name),
                             std::move(rectangle_draw_data));
    }
}

void opengl_renderer::cleanup() {
    for (auto& [identifier, rendering_params] : my_draw_data) {
        rendering_params.cleanup();
        check_gl_error(__FUNCTION__);
    }
    my_draw_data.clear();
}

void opengl_renderer::clear_draw_data() {
    for (auto& [identifier, rendering_params] : my_draw_data) {
        rendering_params.my_vertices_count = 0;
    }
}

void opengl_renderer::render(const material& material, uint32 indices_count,
                             uint32 vertices_offset) const {
    expects(my_draw_data.find(material.my_hash) != my_draw_data.end(),
            "Unknown material cannot be rendered without its parameters");

    const auto& draw_data = my_draw_data.at(material.my_hash);
    draw_data.my_vao.bind();
    {
        glUseProgram(draw_data.my_program);
        glUniform4fv(glGetUniformLocation(draw_data.my_program, "border_color"), 1,
                     &my_pen.my_color[0]);
        glUniform1f(glGetUniformLocation(draw_data.my_program, "border_width"),
                    my_pen.my_line_width);
        glDrawElementsBaseVertex(draw_data.my_primitive, indices_count, GL_UNSIGNED_INT, nullptr,
                                 vertices_offset);
    }
    draw_data.my_vao.unbind();
    glUseProgram(0);
}

void opengl_renderer::render_lines(const program& program, uint32 indices_count,
                                   uint32 vertices_offset) const {
    expects(my_pen.my_line_width > 0.f, "Cannot draw a line with width less than or equal to zero");
    float previous_line_width = 0.f;
    glGetFloatv(GL_LINE_WIDTH, &previous_line_width);

    glLineWidth(my_pen.my_line_width);
    render(material{program, hash(program.my_name)}, indices_count, vertices_offset);
    glLineWidth(previous_line_width);
    check_gl_error(__FUNCTION__);
}

const opengl_draw_data& opengl_renderer::get_draw_data(uint32 id) const {
    return my_draw_data.at(id);
}

const pen& opengl_renderer::get_pen() const { return my_pen; }

void opengl_renderer::set_pen(pen pen) { my_pen = std::move(pen); }

const brush& opengl_renderer::get_brush() const { return my_brush; }

void opengl_renderer::set_brush(brush brush) { my_brush = std::move(brush); }

void draw(opengl_renderer& renderer, const linef& line) {
    const auto& geometry = renderer.get_draw_data(line_data);
    const uint32 vertex_offset = geometry.my_vertices_count;
    renderer.add_draw_data(line_data, to_vertices(line, renderer.get_pen().my_color, no_texture));
    check_gl_error(__FUNCTION__);

    renderer.render_lines(geometry.my_program, nb_points_line, vertex_offset);
}

void draw(opengl_renderer& renderer, const array<linef>& lines) {
    const uint32 nb_points = static_cast<uint32>(lines.size()) * nb_points_line;
    array<vertex> vertices{};
    vertices.reserve(nb_points);

    const auto& geometry = renderer.get_draw_data(line_data);
    const uint32 vertex_offset = geometry.my_vertices_count;
    for (const auto& line : lines) {
        const auto line_vertices = to_vertices(line, renderer.get_pen().my_color, no_texture);
        vertices.insert(vertices.end(), line_vertices.begin(), line_vertices.end());
    }
    renderer.add_draw_data(line_data, vertices);
    check_gl_error(__FUNCTION__);

    renderer.render_lines(geometry.my_program, nb_points, vertex_offset);
}

void draw(opengl_renderer& renderer, const rectf& rect) {
    const auto& geometry = renderer.get_draw_data(rectangle_data);
    const uint32 vertex_offset = geometry.my_vertices_count;
    renderer.add_draw_data(
        rectangle_data, to_vertices(rect, renderer.get_brush().my_fill_color,
                                    static_cast<float>(renderer.get_brush().my_fill_texture.index())));
    check_gl_error(__FUNCTION__);

    renderer.render(material{geometry.my_program.my_gl_id, hash(geometry.my_program.my_name)},
                    nb_indices_rect, vertex_offset);
}

void draw(opengl_renderer& renderer, const array<rectf>& rects) {
    array<vertex> vertices{};
    vertices.reserve(rects.size() * nb_points_rect);

    auto& geometry = renderer.get_draw_data(rectangle_data);
    const uint32 vertex_offset = geometry.my_vertices_count;
    for (const auto& rect : rects) {
        const auto rect_vertices =
            to_vertices(rect, renderer.get_brush().my_fill_color,
                        static_cast<float>(renderer.get_brush().my_fill_texture.index()));
        vertices.insert(vertices.end(), rect_vertices.begin(), rect_vertices.end());
    }
    renderer.add_draw_data(rectangle_data, vertices);
    check_gl_error(__FUNCTION__);

    renderer.render(material{geometry.my_program.my_gl_id, hash(geometry.my_program.my_name)},
                    static_cast<uint32>(rects.size()) * nb_indices_rect, vertex_offset);
}

void clear(opengl_renderer& renderer, const color& color) {
    glClearColor(color.r, color.g, color.b, color.a);
    glClear(GL_COLOR_BUFFER_BIT);
    renderer.clear_draw_data();
}

void draw(opengl_renderer& renderer, const sprite& sprite) {
    auto& geometry = renderer.get_draw_data(rectangle_data);
    const uint32 vertex_offset = geometry.my_vertices_count;
    renderer.add_draw_data(
        rectangle_data,
        to_vertices(sprite, sprite.my_color, static_cast<float>(sprite.my_texture.index())));
    check_gl_error(__FUNCTION__);

    renderer.render(material{geometry.my_program.my_gl_id, hash(geometry.my_program.my_name)},
                    nb_indices_rect, vertex_offset);
}

void draw(opengl_renderer& renderer, const array<sprite>& sprites) {
    array<vertex> vertices{};
    vertices.reserve(sprites.size() * nb_points_rect);

    auto& geometry = renderer.get_draw_data(rectangle_data);
    const uint32 vertex_offset = geometry.my_vertices_count;
    for (const auto& sprite : sprites) {
        const auto sprite_vertices = to_vertices(sprite.my_bounds, sprite.my_color,
                                                 static_cast<float>(sprite.my_texture.index()));
        vertices.insert(vertices.end(), sprite_vertices.begin(), sprite_vertices.end());
    }
    renderer.add_draw_data(rectangle_data, vertices);
    check_gl_error(__FUNCTION__);

    renderer.render(material{geometry.my_program.my_gl_id, hash(geometry.my_program.my_name)},
                    static_cast<uint32>(sprites.size()) * nb_indices_rect, vertex_offset);
}

} // namespace kairos