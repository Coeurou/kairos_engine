#pragma once

#include <buffer_object.h>

#include <nameof.hpp>

void buffer_object::bind() const {
    glBindBuffer(as_gl_target(my_target), my_gl_id);
}

void buffer_object::cleanup() {
    glDeleteBuffers(1, &my_gl_id);
    my_gl_id = 0;
}

GLenum buffer_object::as_gl_target(buffer_target target) {
    GLenum gl_target = GL_INVALID_ENUM;
    switch (target) {
    case buffer_target::geometry: {
        gl_target = GL_ARRAY_BUFFER;
        break;
    }
    case buffer_target::indices: {
        gl_target = GL_ELEMENT_ARRAY_BUFFER;
        break;
    }
    default: {
        log_error(LoggerName::GRAPHICS, "Unhandled buffer target {}", NAMEOF_ENUM(target));
        break;
    }
    }

    return gl_target;
}