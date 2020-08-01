#pragma once

#include <texture.h>
#include <types.h>

struct brush {
    texture fill_texture{};
    color fill_color{ 0.f };
    float opacity = 0.f;
    //gradient gradient {0.f, 0.f};
};