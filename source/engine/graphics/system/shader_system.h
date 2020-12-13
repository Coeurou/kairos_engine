#pragma once

#include <math/mathlib.h>
#include <core/types.h>

class shader_system {
public:
    static bool setup_shaders();
    static bool setup_vaos();
};

struct matrices {
    mat4f my_model{ identity<mat4f>() };
    mat4f my_view{ identity<mat4f>() };
    mat4f my_projection{ identity<mat4f>() };
};
inline matrices our_matrices;
