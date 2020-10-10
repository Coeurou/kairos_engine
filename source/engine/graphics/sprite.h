#pragma once

#include <formattable.h>
#include <rect.h>
#include <texture.h>
#include <types.h>

struct sprite {
    texture* my_texture{};
    rectf my_bounds{{0.f, 0.f}, {0.f, 0.f}};
    rect<int> my_texture_rect{{0, 0}, {0, 0}};
    color my_color{1.f};
    pointf my_origin{0.f};
    float my_rotation{0.f};
};

template <> inline string to_string(sprite s) {
    return fmt::format("Sprite: {}, {}, {}, {}, {}, {}",
                       (s.my_texture != nullptr) ? to_string(*s.my_texture) : "no texture",
                       to_string(s.my_bounds),
                       to_string(s.my_texture_rect), to_string(s.my_color),
                       to_string(s.my_origin), to_string(s.my_rotation));
}

    
inline static_array<pointf, 4> rotate(const sprite& sprite) {
    float angle = glm::radians(sprite.my_rotation);
    const pointf rotation_center = sprite.my_bounds.my_top_left + sprite.my_origin;

    const auto rotate_point = [](const pointf& p, float angle, const pointf& rotation_center) {
        return pointf{rotation_center.x + (p.x - rotation_center.x) * cos(angle) -
                          (p.y - rotation_center.y) * sin(angle),
                      rotation_center.y + (p.x - rotation_center.x) * sin(angle) +
                          (p.y - rotation_center.y) * cos(angle)};
    };

    const rectf& bounds = sprite.my_bounds;
    return {rotate_point(pointf(bounds.right(), bounds.top()), angle, rotation_center),
            rotate_point(bounds.my_bottom_right, angle, rotation_center),
            rotate_point(pointf(bounds.left(), bounds.bottom()), angle, rotation_center),
            rotate_point(bounds.my_top_left, angle, rotation_center)};
}