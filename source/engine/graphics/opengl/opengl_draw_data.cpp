#pragma once

#include <opengl_draw_data.h>

opengl_draw_data::opengl_draw_data(opengl_draw_data&& other) noexcept :
    my_indices_count(std::move(other.my_indices_count)),
    my_vertices_count(std::move(other.my_vertices_count)),
    my_vao(std::move(other.my_vao)),
    my_vbo(std::move(other.my_vbo)),
    my_ibo(std::move(other.my_ibo)),
    my_program(std::move(other.my_program)),
    my_primitive(std::move(other.my_primitive)) {
}

opengl_draw_data& opengl_draw_data::operator=(opengl_draw_data&& other) noexcept {
    my_indices_count = std::move(other.my_indices_count);
    my_vertices_count = std::move(other.my_vertices_count);
    my_vao = std::move(other.my_vao);
    my_vbo = std::move(other.my_vbo);
    my_ibo = std::move(other.my_ibo);
    my_program = std::move(other.my_program);
    my_primitive = std::move(other.my_primitive);
    return *this;
}

void opengl_draw_data::cleanup() {
    my_vao.cleanup();
    my_vbo.cleanup();
    my_ibo.cleanup();
    my_program.cleanup();
    my_indices_count = 0;
    my_vertices_count = 0;
    my_primitive = GL_TRIANGLES;
}