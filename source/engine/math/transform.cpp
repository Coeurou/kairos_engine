#pragma once

#include <transform.h>

#include <glm/gtc/matrix_transform.hpp>

#include <nameof.hpp>

void transform::to_identity()
{
    my_matrix = glm::identity<mat4f>();
}

bool transform::is_identity() const
{
    return my_matrix == glm::identity<mat4f>();
}

void transform::transpose()
{
    my_matrix = glm::transpose(my_matrix);
}

transform transform::get_transposed() const
{
    transform res{};
    res.transpose();
    return res;
}

void transform::set_matrix(const mat4f& matrix)
{
    my_matrix = matrix;
}

pointf transform::map(const pointf& p) const
{
    return vec4f(p.x, p.y, 0.f, 0.f) * my_matrix;
}

linef transform::map(const linef& line) const
{  
    return linef{ map(line.first), map(line.second) };
}

rectf transform::map(const rectf& rectangle) const
{
    return rectf{ map(rectangle.my_top_left), map(rectangle.my_bottom_right) };
}

transform& transform::rotate(float angle_in_degrees, axis axis)
{
    my_matrix *= glm::rotate(mat4f(1.0f), glm::radians(angle_in_degrees), to_vec3f(axis));
    return *this;
}

transform& transform::rotate_radians(float angle_in_radians, axis axis)
{
    my_matrix *= glm::rotate(mat4f(1.0f), angle_in_radians, to_vec3f(axis));
    return *this;
}

transform& transform::scale(vec2f scale)
{
    my_matrix *= glm::scale(mat4f(1.0f), vec3f(scale, 0.f));
    return *this;
}

transform& transform::scale(float sx, float sy)
{
    return translate(vec2f(sx, sy));
}

transform& transform::translate(vec2f translation)
{
    my_matrix *= glm::translate(mat4f(1.0f), vec3f(translation, 0.f));
    return *this;
}

transform& transform::translate(float dx, float dy)
{
    return translate(vec2f(dx, dy));
}

bool transform::operator==(const transform& other) const
{
    return my_matrix == other.my_matrix;
}

bool transform::operator!=(const transform& other) const
{
    return my_matrix != other.my_matrix;
}

transform transform::operator*(const transform& other) const
{
    transform res = other;
    res.my_matrix *= my_matrix;
    return res;
}

transform transform::operator*(transform&& other) const
{
    transform res = other;
    res.my_matrix *= my_matrix;
    return res;
}

transform& transform::operator*=(const transform& other)
{
    my_matrix *= other.my_matrix;
    return *this;
}

transform& transform::operator*=(float scalar)
{
    my_matrix *= scalar;
    return *this;
}

transform& transform::operator+=(float scalar)
{
    my_matrix += scalar;
    return *this;
}

transform& transform::operator-=(float scalar)
{
    my_matrix -= scalar;
    return *this;
}

transform& transform::operator/=(float scalar)
{
    my_matrix /= scalar;
    return *this;
}

transform transform::identity()
{
    transform res{};
    res.my_matrix = glm::identity<mat4f>();
    return res;
}

transform transform::from_scale(vec2f scale)
{
    transform res{};
    res.my_matrix = glm::scale(mat4f(1.0f), vec3f(scale, 0.f));
    return res;
}

transform transform::from_scale(float sx, float sy)
{
    return from_scale(vec2f{ sx, sy });
}

transform transform::from_translate(vec2f translation)
{
    transform res{};
    res.my_matrix = glm::translate(mat4f(1.0f), vec3f(translation, 0.f));
    return res;
}

transform transform::from_translate(float dx, float dy)
{
    return from_translate(vec2f{dx, dy});
}

vec3f to_vec3f(axis axis)
{
    vec3f res{ 0.f };
    switch (axis)
    {
    case axis::x_axis:
        res.x = 1.f;
        break;
    case axis::y_axis:
        res.y = 1.f;
        break;
    case axis::z_axis:
        res.z = 1.f;
        break;
    default:
        ensures(false, fmt::format("Unimplemented axis enum, you shouldn't enter here: {}\n", NAMEOF_ENUM(axis)).c_str());
        break;
    }
    return res;
}
