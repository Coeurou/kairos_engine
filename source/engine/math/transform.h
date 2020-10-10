#pragma once

#include <rect.h>
#include <types.h>

enum class axis {
    x_axis,
    y_axis,
    z_axis
};
static vec3f to_vec3f(axis axis);

class transform {
public:
    void to_identity();
    bool is_identity() const;
    void transpose();
    transform get_transposed() const;
    void set_matrix(const mat4f& matrix);

    pointf map(const pointf& p) const;
    linef map(const linef& line) const;
    rectf map(const rectf& rectangle) const;

    transform& rotate(float angle_in_degrees, axis axis);
    transform& rotate_radians(float angle_in_radians, axis axis);
    transform& scale(vec2f scale);
    transform& scale(float sx, float sy);
    transform& translate(vec2f translation);
    transform& translate(float dx, float dy);

    bool operator==(const transform& other) const;
    bool operator!=(const transform& other) const;
    transform operator*(const transform& other) const;
    transform operator*(transform&& other) const;
    transform& operator*=(const transform& other);
    transform& operator*=(float scalar);
    transform& operator+=(float scalar);
    transform& operator-=(float scalar);
    transform& operator/=(float scalar);

    static transform identity();
    static transform from_scale(vec2f scale);
    static transform from_scale(float sx, float sy);
    static transform from_translate(vec2f translation);
    static transform from_translate(float dx, float dy);

private:
    mat4f my_matrix{1.f};
};