#pragma once

#include <types.h>

struct shader {
    uint32 my_gl_id{ 0 };
    GLenum my_shader_type{ GL_NONE };
    string my_shader_name{ "" };
    string my_shader_source{ "" };

    ~shader();
    bool setup();
    bool compile();
    void cleanup();
};

inline array<shader> our_vertex_shaders;
inline array<shader> our_fragment_shaders;
