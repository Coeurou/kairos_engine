#pragma once

#include <cmath>
#include <glm/gtx/transform.hpp>

#include <math/rect.h>
#include <core/types.h>

namespace kairos {

float square_distance(const vec2f& point_a, const vec2f& point_b);

template <class matType> matType identity() { return glm::identity<matType>(); }

template <typename number> constexpr number to_radians(number degrees) {
    return glm::radians(degrees);
}

template <typename number> constexpr number to_degrees(number radians) {
    return glm::degrees(radians);
}

mat4f ortho(const rectf& bounds);

} // namespace kairos