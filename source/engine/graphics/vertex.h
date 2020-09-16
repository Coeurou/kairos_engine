#pragma once

#include <types.h>

struct vertex {
    vec3f position{ 0.f };
    color color{ 1.f };
    vec2f tex_coords{ 0.f };
    uint32 texture_id{ 0 };
};