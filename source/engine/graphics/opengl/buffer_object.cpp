#pragma once

#include <buffer_object.h>

#include <nameof.hpp>

#include <contract.h>

buffer_object::buffer_object(buffer_object&& other) noexcept :
    my_gl_id(std::move(other.my_gl_id)), my_usage(std::move(other.my_usage)),
    my_target(std::move(other.my_target)), my_size(std::move(other.my_size)) {
    other.my_gl_id = 0;
    other.my_target = buffer_target::count;
    other.my_usage = GL_STATIC_DRAW;
    other.my_size = 0;
}

buffer_object& buffer_object::operator=(buffer_object&& other) noexcept {
    my_gl_id = std::move(other.my_gl_id);
    my_usage = std::move(other.my_usage);
    my_target = std::move(other.my_target);
    my_size = std::move(other.my_size);

    other.my_gl_id = 0;
    other.my_target = buffer_target::count;
    other.my_usage = GL_STATIC_DRAW;
    other.my_size = 0;
    return *this;
}

void buffer_object::bind() const {
    glBindBuffer(as_gl_target(my_target), my_gl_id);
}

void buffer_object::unbind() const {
    glBindBuffer(as_gl_target(my_target), 0);
}

void buffer_object::cleanup() {
    if (my_gl_id != 0) {
        glDeleteBuffers(1, &my_gl_id);
        my_gl_id = 0;
        my_target = buffer_target::count;
    }
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
        ensures(false, fmt::format("Unhandled buffer target {}", NAMEOF_ENUM(target)));
        break;
    }
    }

    return gl_target;
}