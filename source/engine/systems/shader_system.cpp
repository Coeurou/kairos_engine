#pragma once

#include <shader_system.h>
#include <program.h>
#include <shader.h>
#include <types.h>
#include <vao.h>

bool shader_system::setup_shaders() {
    bool res = true;

    shader vertex_shader;
    vertex_shader.my_shader_type = GL_VERTEX_SHADER;
    vertex_shader.my_shader_name = "default_2d_vs";
    vertex_shader.my_shader_source = R"(
    #version 330
    in vec3 position;
    in vec4 color;
    in vec2 texcoords;
    in int texid;

    out vec4 vs_color;
    out vec2 vs_texcoords;
    out int vs_texid;

    uniform mat4 mvp_matrix;

    void main() {
        vs_color = color;
        vs_texcoords = texcoords;
        vs_texid = texid;
        gl_Position = mvp_matrix * vec4(position, 1.0f);
    })";
    res &= vertex_shader.setup();
    our_vertex_shaders.emplace_back(vertex_shader);

    shader fragment_shader;
    fragment_shader.my_shader_type = GL_FRAGMENT_SHADER;
    fragment_shader.my_shader_name = "default_2d_fs";
    fragment_shader.my_shader_source = R"(
    #version 330
    in vec4 vs_color;
    in vec2 vs_texcoords;
    in int vs_texid;

    out vec4 frag_color;

    uniform sampler2D textures[8];

    void main() {
        vs_color = color;
        vs_texcoords = texcoords;
        vs_texid = texid;
        frag_color = texture(textures[vs_texid], vs_texcoords) * vs_color;
    })";
    res &= fragment_shader.setup();
    our_fragment_shaders.emplace_back(fragment_shader);

    program program;
    program.my_name = "default_program";
    res &= program.setup({ vertex_shader.my_gl_id, fragment_shader.my_gl_id });
    our_programs.emplace_back(program);

    return res;
}

bool shader_system::setup_vaos() {
    bool res = true;
    vao vao;
    vao::setup(vao);

    our_vaos.emplace_back(vao);

    return !our_vaos.empty();
}