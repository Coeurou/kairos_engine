#pragma once

#include <math/rect.h>
#include <graphics/sprite.h>
#include <core/types.h>

template <class Shape> class transform {
public:
    auto rotate(const Shape& shape, float angle_in_degrees, const pointf& origin) {
        return rotate_radians(shape, glm::radians(angle_in_degrees), origin);
    }

    auto rotate_radians(const Shape& shape, float angle_in_radians, const pointf& origin) {
        if constexpr (std::is_same_v<std::decay_t<Shape>, pointf>) {
            const float cos_theta = std::cosf(angle_in_radians);
            const float sin_theta = std::sinf(angle_in_radians);
            return pointf{
                origin.x + ((shape.x - origin.x) * cos_theta - (shape.y - origin.y) * sin_theta),
                origin.y + ((shape.x - origin.x) * sin_theta + (shape.y - origin.y) * cos_theta)};
        }
        else if constexpr (std::is_same_v<std::decay_t<Shape>, linef>) {
            return linef{rotate_radians(shape.first, angle_in_radians, origin),
                         rotate_radians(shape.second, angle_in_radians, origin)};
        }
        else if constexpr (std::is_same_v<std::decay_t<Shape>, sprite>) {
            return glm::degrees(angle_in_radians);
        }
        else {
            static_assert(false, "Shape doesn't support rotation");
        }
    }

    auto scale(const Shape& shape, float s, const pointf& origin) {
        if constexpr (std::is_same_v<std::decay_t<Shape>, linef>) {
            const vec2f scale_first = (origin - shape.first) * s;
            const vec2f scale_second = (origin - shape.second) * s;
            return linef{shape.first + scale_first, shape.second + scale_second};
        }
        else if constexpr (std::is_same_v<std::decay_t<Shape>, rectf>) {
            const vec2f scale_top_left = (origin - shape.my_top_left) * s;
            const vec2f scale_bottom_right = (origin - shape.my_bottom_right) * s;
            return rectf {
                shape.my_top_left + scale_top_left, shape.my_bottom_right + scale_bottom_right
            }
        }
        else if constexpr (std::is_same_v<std::decay_t<Shape>, sprite>) {
            return scale(shape.my_bounds, s, origin);
        }
        else {
            static_assert(false, "Shape doesn't support scaling");
        }
    }

    auto translate(const Shape& shape, const vec2f& translation) {
        if constexpr (std::is_same_v<std::decay_t<Shape>, pointf>) {
            return shape + translation;
        } else if constexpr (std::is_same_v<std::decay_t<Shape>, linef>) {
            return linef{translate(shape.first, dx, dy), translate(shape.second, dx, dy)};
        } else if constexpr (std::is_same_v<std::decay_t<Shape>, rectf>) {
            return shape.translated(translation);
        } else if constexpr (std::is_same_v<std::decay_t<Shape>, sprite>) {
            return translate(shape.my_bounds, translation);
        } else {
            static_assert(false, "Shape doesn't support translation");
        }
    }

    auto translate(const Shape& shape, float dx, float dy) {
        return translate(vec2f{dx, dy});
    }
};