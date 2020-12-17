#pragma once

#include <math/rect.h>
#include <graphics/texture.h>
#include <core/types.h>

namespace kairos {

class sprite {
  public:
    texture my_texture{};
    rectf my_bounds{{0.f, 0.f}, {0.f, 0.f}};
    rect<int> my_texture_rect{{0, 0}, {0, 0}};
    color my_color{1.f};
    float my_rotation{0.f};
    pointf my_origin{0.f};

    // function recommanded for setting sprite anchor/origin point
    // we have to make sure the point is located in local coordinates and do a conversion if not
    void set_origin(const pointf& new_origin);
};

/** Non-member functions */

string to_string(const sprite& s);
void translate(sprite& s, const pointf& pos);
void rotate(sprite& s, float angle_in_degrees);
void rotate_radians(sprite& s, float angle_in_radians);

} // namespace kairos