#pragma once

#include <core/types.h>

namespace kairos {

static constexpr float default_line_width = 1.f;

struct pen {
    color my_color{1.f};
    float my_line_width{0.f};
    float my_opacity{0.f};
};

} // namespace kairos