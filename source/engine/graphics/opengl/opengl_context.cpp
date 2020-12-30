#pragma once

#include <graphics/opengl/opengl_context.h>

#include <core/contract.h>

namespace kairos {

/** Member functions */

opengl_context::opengl_context(const opengl_context& gl) { gl.get()->copy(my_memory.data()); }

opengl_context& opengl_context::operator=(const opengl_context& gl) {
    this->~opengl_context();
    gl.get()->copy(my_memory.data());

    return *this;
}

opengl_context::opengl_context(opengl_context&& gl) noexcept {
    gl.get()->move(my_memory.data());
    gl.my_memory = {'\0'};
}

opengl_context& opengl_context::operator=(opengl_context&& gl) noexcept {
    this->~opengl_context();
    gl.get()->move(my_memory.data());

    return *this;
}

opengl_context::~opengl_context() {
    if (std::any_of(std::begin(my_memory), std::end(my_memory), [](char c) { return c != '\0'; })) {
        get()->destruct();
        my_memory = {'\0'};
    }
}

opengl_context::opengl_context_t* opengl_context::get() {
    return reinterpret_cast<opengl_context_t*>(my_memory.data());
}

const opengl_context::opengl_context_t* opengl_context::get() const {
    return reinterpret_cast<const opengl_context_t*>(my_memory.data());
}

/** Non-member functions */

bool setup(opengl_context& gl_context, uint32 window_id) { return gl_context.get()->setup(window_id); }

void swap_buffers(opengl_context& gl_context) { gl_context.get()->swap_buffers(); }

int version(const opengl_context& gl_context) { return gl_context.get()->version(); }

int get_attribute(const opengl_context& gl_context, gl_attribute attr) {
    return gl_context.get()->get_attribute(attr);
}

void set_attribute(opengl_context& gl_context, gl_attribute attr, int value) {
    gl_context.get()->set_attribute(attr, value);
}

void destroy(opengl_context& gl_context) {
    gl_context.get()->destroy();
}

/** Utils */
void check_gl_error(const char* caller) {
    const auto error = glGetError();
    ensures(error == GL_NO_ERROR, fmt::format("{} raises a GL error: {}", caller, gl_error_to_string.at(error)));
}

} // namespace kairos