#pragma once

#include <types.h>

struct vertex {
    vec4f my_position{ 0.f };
    color my_color{ 1.f };
    vec2f my_tex_coords{ 0.f };
    float my_texture_id{ -1.f };
};