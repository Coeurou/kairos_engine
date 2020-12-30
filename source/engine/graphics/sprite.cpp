#include <graphics/sprite.h>

#include <core/formattable.h>
#include <math/mathlib.h>
#include "sprite.h"

namespace kairos {

void sprite::set_origin(const pointf& new_origin) {
    if (contains(my_bounds, new_origin)) {
        my_origin = new_origin;
    } else {
        my_origin = (new_origin - my_bounds.my_top_left);
    }
}

string to_string(const sprite& s) {
    return fmt::format("Sprite: {}, {}, {}, {}, {}, {}",
        to_string(s.my_texture),
        to_string(s.my_bounds), to_string(s.my_texture_rect), to_string(s.my_color),
        to_string(s.my_origin), to_string(s.my_rotation));
}

void translate(sprite& s, const pointf& pos) {
    translate(s.my_bounds, pos);
    s.set_origin(center(s.my_bounds));
}

void rotate(sprite& s, float angle_in_degrees) {
    rotate_radians(s, to_radians(angle_in_degrees));
}

void rotate_radians(sprite& s, float angle_in_radians) {
    s.my_rotation = angle_in_radians;
}

void flip(sprite& s, axis axis, bool value) {
    s.my_flip[static_cast<size_t>(axis)] = value;
}

} // namespace kairos