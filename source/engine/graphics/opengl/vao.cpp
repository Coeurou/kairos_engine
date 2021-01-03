#pragma once

#include <graphics/opengl/vao.h>

#include <core/contract.h>
#include <core/algorithms.h>
#include <graphics/vertex.h>

namespace kairos {

vao::vao() { glGenVertexArrays(1, &my_gl_id); }

vao::vao(vao&& other) noexcept
    : my_gl_id(std::move(other.my_gl_id)), my_name(std::move(other.my_name)) {
    other.my_gl_id = 0;
    other.my_name = "";
}

vao& vao::operator=(vao&& other) noexcept {
    my_gl_id = std::move(other.my_gl_id);
    my_name = std::move(other.my_name);
    other.my_gl_id = 0;
    other.my_name = "";
    return *this;
}

void vao::bind() const { glBindVertexArray(my_gl_id); }

void vao::unbind() const {
    GLint current_vao;
    glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &current_vao);
    if (current_vao > 0 && static_cast<uint32>(current_vao) == my_gl_id) {
        glBindVertexArray(0);
    }
}

void vao::setup() const {
    glVertexAttribPointer(position_attrib_index, 4, GL_FLOAT, GL_FALSE, sizeof(vertex),
                          (void*)offsetof(vertex, my_position));
    glEnableVertexAttribArray(position_attrib_index);
    glVertexAttribPointer(color_attrib_index, 4, GL_FLOAT, GL_FALSE, sizeof(vertex),
                          (void*)offsetof(vertex, my_color));
    glEnableVertexAttribArray(color_attrib_index);
    glVertexAttribPointer(texcoords_attrib_index, 2, GL_FLOAT, GL_FALSE, sizeof(vertex),
                          (void*)offsetof(vertex, my_tex_coords));
    glEnableVertexAttribArray(texcoords_attrib_index);
    glVertexAttribPointer(tex_id_attrib_index, 1, GL_FLOAT, GL_FALSE, sizeof(vertex),
                          (void*)offsetof(vertex, my_texture_id));
    glEnableVertexAttribArray(tex_id_attrib_index);
}

void vao::cleanup() {
    if (my_gl_id != 0) {
        bind();
        glDisableVertexAttribArray(position_attrib_index);
        glDisableVertexAttribArray(color_attrib_index);
        glDisableVertexAttribArray(texcoords_attrib_index);
        glDisableVertexAttribArray(tex_id_attrib_index);
        unbind();
        glDeleteVertexArrays(1, &my_gl_id);
        my_gl_id = 0;
        if (auto vao_to_delete = remove(our_vaos, *this); vao_to_delete != our_vaos.end()) {
            our_vaos.erase(vao_to_delete);
        }
    }
}

vao* vao::find(string_view name) {
    auto vao_it = find_if(our_vaos, [&name](vao& vao) { return vao.my_name == name; });
    ensures(vao_it != our_vaos.end(), "Couldn't find the vertex array object in container");
    return &(*vao_it);
}

} // namespace kairos