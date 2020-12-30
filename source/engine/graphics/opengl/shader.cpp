#pragma once

#include <graphics/opengl/shader.h>

#include <core/logger.h>

shader::shader(shader&& other) noexcept :
    my_gl_id(std::move(other.my_gl_id)), my_shader_name(std::move(other.my_shader_name)),
    my_shader_source(std::move(other.my_shader_source)), my_shader_type(std::move(other.my_shader_type)) {
    other.my_gl_id = 0;
    other.my_shader_name = "";
    other.my_shader_source = "";
    other.my_shader_type = GL_NONE;
}

shader& shader::operator=(shader&& other) noexcept {
    my_gl_id = std::move(other.my_gl_id);
    my_shader_name = std::move(other.my_shader_name);
    my_shader_source = std::move(other.my_shader_source);
    my_shader_type = std::move(other.my_shader_type);

    other.my_gl_id = 0;
    other.my_shader_name = "";
    other.my_shader_source = "";
    other.my_shader_type = GL_NONE;
    return *this;
}

shader::~shader() {
    cleanup();
}

bool shader::setup() {
    my_gl_id = glCreateShader(my_shader_type);
    const char* source = my_shader_source.data();
    glShaderSource(my_gl_id, 1, &source, nullptr);

    return compile();
}

bool shader::compile() const {
    glCompileShader(my_gl_id);
    int success = GL_FALSE;
    glGetShaderiv(my_gl_id, GL_COMPILE_STATUS, &success);
    if (success == GL_FALSE) {
        static_array<char, 512> info_log = { 0 };
        glGetShaderInfoLog(my_gl_id, static_cast<GLsizei>(count(info_log)), nullptr, info_log.data());
        log_error(LoggerName::GRAPHICS, "Shader compilation failed: {}\n", info_log.data());
    }

    return (success == GL_TRUE);
}

void shader::cleanup() {
    if (my_gl_id != 0) {
        glDeleteShader(my_gl_id);
        my_gl_id = 0;
        my_shader_name = "";
        my_shader_source = "";
        my_shader_type = GL_NONE;
    }
}
