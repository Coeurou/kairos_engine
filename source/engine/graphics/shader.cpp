#pragma once

#include <shader.h>

#include <logger.h>

shader::~shader() {
    cleanup();
}

bool shader::setup() {
    my_gl_id = glCreateShader(my_shader_type);
    const char* source = my_shader_source.data();
    glShaderSource(my_gl_id, 1, &source, nullptr);

    return compile();
}

bool shader::compile() {
    glCompileShader(my_gl_id);
    int success = GL_FALSE;
    glGetShaderiv(my_gl_id, GL_COMPILE_STATUS, &success);
    if (success == GL_FALSE) {
        static_array<char, 512> info_log = { 0 };
        glGetShaderInfoLog(my_gl_id, info_log.size(), nullptr, info_log.data());
        log_error(LoggerName::GRAPHICS, "Shader compilation failed: {}\n", info_log.data());
    }

    return (success == GL_TRUE);
}

void shader::cleanup() {
    glDeleteShader(my_gl_id);
    my_gl_id = 0;
}
