#pragma once

#include <cmath>

#include <types.h>
#include <glm/gtx/transform.hpp>

float square_distance(const vec2f& point_a, const vec2f& point_b);

template<class matType> matType identity() { return glm::identity<matType>(); }