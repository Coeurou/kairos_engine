#pragma once

#include <cmath>
#include <glm/gtx/transform.hpp>

#include <rect.h>
#include <types.h>

float square_distance(const vec2f& point_a, const vec2f& point_b);

template<class matType> matType identity() { return glm::identity<matType>(); }
mat4f ortho(const rectf& bounds);