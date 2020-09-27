#pragma once

#include <texture.h>
#include <types.h>

struct brush {
    int my_fill_texture_id{-1};
    color my_fill_color{ 0.f };
    float my_opacity{ 0.f };
    //gradient my_gradient {0.f, 0.f};
};