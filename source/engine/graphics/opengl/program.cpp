#pragma once

#include <glad/glad.h>

#include <graphics/opengl/program.h>

#include <core/contract.h>
#include <core/logger.h>

namespace kairos {

program::program(program&& other) noexcept
    : my_gl_id(std::move(other.my_gl_id)), my_name(std::move(other.my_name)),
      my_shaders(std::move(other.my_shaders)) {
    other.my_gl_id = 0;
    other.my_name = "";
    other.my_shaders = {};
}

program& program::operator=(program&& other) noexcept {
    my_gl_id = std::move(other.my_gl_id);
    my_name = std::move(other.my_name);
    my_shaders = std::move(other.my_shaders);
    other.my_gl_id = 0;
    other.my_name = "";
    other.my_shaders = {};
    return *this;
}

program::~program() { cleanup(); }

bool program::setup(std::initializer_list<uint32> shaders) {
    expects(shaders.size() <= program::max_shaders, "program::setup: Too many shaders provided");

    my_gl_id = glCreateProgram();

    for (uint32 shader : shaders) {
        glAttachShader(my_gl_id, shader);
    }

    return link();
}

bool program::link() const {
    glLinkProgram(my_gl_id);
    int success = GL_FALSE;
    glGetProgramiv(my_gl_id, GL_LINK_STATUS, &success);
    if (success == GL_FALSE) {
        static_array<char, 512> info_log = {0};
        glGetProgramInfoLog(my_gl_id, static_cast<GLsizei>(count(info_log)), nullptr,
                            info_log.data());
        log_error(LoggerName::GRAPHICS, "Program link failed: {}\n", info_log.data());
    }

    return (success == GL_TRUE);
}

void program::cleanup() {
    if (my_gl_id != 0) {
        for (auto shader_id : my_shaders) {
            if (shader_id != 0) {
                glDetachShader(my_gl_id, shader_id);
                glDeleteShader(shader_id);
            }
        }
        glDeleteProgram(my_gl_id);
        my_gl_id = 0;
    }
}

} // namespace kairos