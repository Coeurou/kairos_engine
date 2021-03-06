#pragma once

#include <cmath>
#include <glm/gtx/transform.hpp>

#include <core/types.h>
#include <math/rect.h>

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

float length(const vec2f& v) {
    return glm::length(v);
}

vec2f normalize(const vec2f& v)
{
    return glm::normalize(v);
}

float square_length(const vec2f& v) {
    return v.x * v.x + v.y * v.y;
}

uint32 square_length(const vec2i& v) {
    return static_cast<uint32>(v.x * v.x + v.y * v.y);
}

template <class Vector> Vector normalize(const Vector& v) { return glm::normalize(v); }

template <class Vector> void normalized(Vector& v) { v = glm::normalize(v); }

/** When removing glm dependency this should accept universal references and forward arguments */
template <size_t L, class Arithmetic>
Arithmetic dot(const vec<L, Arithmetic>& v1, const vec<L, Arithmetic>& v2) {
    return glm::dot(v1, v2);
}

/** When removing glm dependency this should accept universal references and forward arguments */
template <class Vector> Vector cross(const Vector& v1, const Vector& v2) {
    return glm::cross(v1, v2);
}

} // namespace kairos