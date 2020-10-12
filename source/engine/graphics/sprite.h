#pragma once

#include <formattable.h>
#include <rect.h>
#include <texture.h>
#include <types.h>

class sprite {
public:
    texture* my_texture{};
    rectf my_bounds{{0.f, 0.f}, {0.f, 0.f}};
    rect<int> my_texture_rect{{0, 0}, {0, 0}};
    color my_color{1.f};
    float my_rotation{0.f};
    pointf my_origin{0.f};

    // function recommanded for setting sprite anchor/origin point
    // we have to make sure the point is located in local coordinates and do a conversion if not
    void set_origin(const pointf& new_origin);
};

template <> inline string to_string(sprite s) {
    return fmt::format("Sprite: {}, {}, {}, {}, {}, {}",
                       (s.my_texture != nullptr) ? to_string(*s.my_texture) : "no texture",
                       to_string(s.my_bounds),
                       to_string(s.my_texture_rect), to_string(s.my_color),
                       to_string(s.my_origin), to_string(s.my_rotation));
}