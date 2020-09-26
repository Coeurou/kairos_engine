#include <mathlib.h>

float square_distance(const vec2f& point_a, const vec2f& point_b) {
    const vec2f vec{point_b - point_a};
    return vec.x * vec.x + vec.y * vec.y;
}

mat4f ortho(const rectf& bounds) {
    return glm::ortho(bounds.left(), bounds.right(), bounds.bottom(), bounds.top());
}