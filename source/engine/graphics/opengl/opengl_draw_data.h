#pragma once

#include <types.h>
#include <buffer_object.h>
#include <program.h>
#include <vao.h>

class opengl_draw_data {
public:
    uint32 my_indices_count{ 0 };
    uint32 my_vertices_count{ 0 };
    vao my_vao{};
    buffer_object my_vbo{};
    buffer_object my_ibo{};
    program my_program{};
    GLenum my_primitive{ GL_TRIANGLES };

    opengl_draw_data() = default;

    opengl_draw_data(const opengl_draw_data&) = default;
    opengl_draw_data& operator=(const opengl_draw_data&) = default;

    opengl_draw_data(opengl_draw_data&& other) noexcept;
    opengl_draw_data& operator=(opengl_draw_data&& other) noexcept;

    template<class Vertices, class Indices>
    void setup(Vertices&& vertices, GLenum vbo_usage, Indices&& indices, std::initializer_list<uint32> shaders, string_view name, GLenum primitive) {
        my_vao.bind();
        my_vbo.my_target = buffer_target::geometry;
        my_vbo.my_usage = vbo_usage;
        my_vbo.setup(vertices);
        my_ibo.my_target = buffer_target::indices;
        my_ibo.setup(indices);
        my_vao.setup();
        my_program.setup(shaders);
        my_program.my_name = name;
        my_primitive = primitive;
        my_vao.unbind();
        my_vbo.unbind();
        my_ibo.unbind();
    }

    // Fill a specific part of the VBO and IBO by indexing the count of vertices and indices already in place
    template<class Vertices>
    void add_draw_data(Vertices&& vertices) {
        my_vao.bind();
        my_vbo.bind();
        using vertices_container = std::remove_reference<Vertices>::type;
        using vertex_type = vertices_container::value_type;
        glBufferSubData(buffer_object::as_gl_target(my_vbo.my_target), my_vertices_count * sizeof(vertex_type), vertices.size() * sizeof(vertex_type), vertices.data());
        my_vertices_count += vertices.size();
        check_gl_error();

        my_vao.unbind();
        my_vbo.unbind();
    }

    void cleanup();
};
