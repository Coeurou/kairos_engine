#pragma once

#include <types.h>

struct ibo {
    uint32 my_gl_id{ 0 };
    GLenum my_usage{ GL_STATIC_DRAW };
    const GLenum my_target{ GL_ELEMENT_ARRAY_BUFFER };

    static void setup(ibo& ibo, const array<uint32>& indices);
};