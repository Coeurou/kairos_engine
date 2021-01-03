#pragma once

#include <glad/glad.h>

#include <core/types.h>

namespace kairos {

struct shader {
    uint32 my_gl_id{0};
    GLenum my_shader_type{GL_NONE};
    string my_shader_name{""};
    string my_shader_source{""};

    shader() = default;
    shader(const shader& other) = delete;
    shader& operator=(const shader& other) = delete;

    shader(shader&& other) noexcept;
    shader& operator=(shader&& other) noexcept;

    ~shader();

    operator uint32() const { return my_gl_id; }

    bool setup();
    bool compile() const;
    void cleanup();
};

inline array<shader> our_vertex_shaders;
inline array<shader> our_fragment_shaders;

} // namespace kairos