#pragma once

#include <vao.h>

#include <error.h>
#include <kairos_lib.h>
#include <vertex.h>

void vao::bind() const {
    glBindVertexArray(my_gl_id);
}

void vao::unbind() const {
    glBindVertexArray(0);
}

void vao::set_vertex_buffer(const buffer_object& vbo) const {
    set_buffer(vbo);
}

void vao::set_index_buffer(const buffer_object& ibo) const {
    set_buffer(ibo);
}

void vao::set_buffer(const buffer_object& bo) const {
    ASSERT_MSG(is_bound, "vao::set_buffer: vao not bound");
    bo.bind();
}

void vao::setup(vao& vao) {
    glGenVertexArrays(1, &vao.my_gl_id);

    glVertexAttribPointer(position_attrib_index, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, position));
    glEnableVertexAttribArray(position_attrib_index);
    glVertexAttribPointer(color_attrib_index, 4, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, color));
    glEnableVertexAttribArray(color_attrib_index);
    glVertexAttribPointer(texcoords_attrib_index, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, tex_coords));
    glEnableVertexAttribArray(texcoords_attrib_index);
    glVertexAttribPointer(tex_id_attrib_index, 1, GL_UNSIGNED_INT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, texture_id));
    glEnableVertexAttribArray(tex_id_attrib_index);
}

std::optional<vao> vao::find(string_view name) {
    if (auto vao_it = find_if(our_vaos, [&name](const vao& vao) { return vao.my_name == name; }); vao_it != our_vaos.end()) {
        return *vao_it;
    } 
    else {
        return std::nullopt;
    }
}