#pragma once

#include <core/types.h>
#include <graphics/texture.h>

namespace kairos {

struct brush {
    texture my_fill_texture{};
    color my_fill_color{0.f};
    float my_opacity{0.f};
    // gradient my_gradient {0.f, 0.f};
};

} // namespace kairos