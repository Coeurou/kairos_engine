#pragma once

#include <types.h>

struct vertex {
    vec3f position{ 0.f };
    color color{ 1.f };
    vec2f tex_coords{ 0.f };
    int texture_id{ -1 };
};