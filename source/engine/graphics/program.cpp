#pragma once

#include <program.h>
#include <error.h>
#include <logger.h>

program::~program() {
    cleanup();
}

bool program::setup(std::initializer_list<uint32> shaders) {
    ASSERT_MSG(shaders.size() <= program::max_shaders, "program::setup: Too many shaders provided");

    my_gl_id = glCreateProgram();

    for (uint32 shader : shaders) {
        glAttachShader(my_gl_id, shader);
    }

    return link();
}

bool program::link() {
    glLinkProgram(my_gl_id);
    int success = GL_FALSE;
    glGetProgramiv(my_gl_id, GL_LINK_STATUS, &success);
    if (success == GL_FALSE) {
        static_array<char, 512> info_log = { 0 };
        glGetProgramInfoLog(my_gl_id, info_log.size(), nullptr, info_log.data());
        log_error(LoggerName::GRAPHICS, "Program link failed: {}\n", info_log.data());
    }

    return (success == GL_TRUE);
}

void program::cleanup() {
    for (auto shader_id : my_shaders) {
        glDetachShader(my_gl_id, shader_id);
    }
    glDeleteProgram(my_gl_id);
    my_gl_id = 0;
}
