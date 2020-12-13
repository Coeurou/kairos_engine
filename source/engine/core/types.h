#pragma once

#include <array>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>
#include <unordered_map>
#include <variant>
#include <vector>

#include <fmt/format.h>
#include <glad/glad.h>
#include <glm/glm.hpp>

using vec2i = glm::ivec2;
using vec2f = glm::vec2;
using vec3f = glm::vec3;
using vec4f = glm::vec4;
using color = glm::vec4;
using mat4f = glm::mat4;
using mat3f = glm::mat3;
using int64 = std::int64_t;
using uint64 = std::uint64_t;
using int8 = char;
using uint8 = unsigned char;
using int32 = int;
using uint32 = std::uint32_t;
using sizef = vec2f;
using pointf = vec2f;
using linef = std::pair<vec2f, vec2f>;
template<class T> using array = std::vector<T>;
template<class T, size_t N> using static_array = std::array<T, N>;
template<class Key, class Value> using dictionary = std::unordered_map<Key, Value>;
using string = std::string;
using string_view = std::string_view;
using variant = std::variant<int, uint32, float, size_t, int64, uint64, double, vec2f, vec2i, vec3f, string, string_view>;
using command_line_args = std::unordered_map<string, variant>;

template<class Enum, class Integral, typename = std::enable_if_t<std::is_integral_v<Integral>>> Integral to(Enum value) {
    return static_cast<Integral>(value);
}