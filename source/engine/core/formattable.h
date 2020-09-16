#pragma once

#include <regex>

#include <contract.h>
#include <types.h>

template<class T> string to_string(T value) {
    return std::to_string(value);
}

template<> inline string to_string(string s) {
    return s;
}

template<> inline string to_string(string_view s) {
    return s.data();
}

template<> inline string to_string(vec2f v) {
    return fmt::format("vector 2D - x: {}, y: {}", v.x, v.y);
}

template<> inline string to_string(linef l) {
    return fmt::format("line - first: {}, second: {}", to_string(l.first), to_string(l.second));
}

template<> inline string to_string(const linef& l) {
    return fmt::format("line - first: {}, second: {}", to_string(l.first), to_string(l.second));
}

template<> inline string to_string(vec2i v) {
    return fmt::format("vector 2D - x: {}, y: {}", v.x, v.y);
}

template<> inline string to_string(vec3f v) {
    return fmt::format("vector 3D - x: {}, y: {}, z: {}", v.x, v.y, v.z);
}

template<> inline string to_string(const vec3f& v) {
    return fmt::format("vector 3D - x: {}, y: {}, z: {}", v.x, v.y, v.z);
}

template<> inline string to_string(vec4f v) {
    return fmt::format("vector 4D - x: {}, y: {}, z: {}, w: {}", v.x, v.y, v.z, v.w);
}

template<> inline string to_string(const vec4f& v) {
    return fmt::format("vector 4D - x: {}, y: {}, z: {}, w: {}", v.x, v.y, v.z, v.w);
}

template<> inline string to_string(const variant* v) {
    return std::visit([](auto&& arg) {
        return to_string(arg);
    }, *v);
}

template<> inline string to_string(variant* v) {
    return to_string(static_cast<const variant*>(v));
}

template<class T> T from_string(string s) {
    return T{};
}

template<> inline string from_string(string s) {
    return s;
}

template<> inline string_view from_string(string s) {
    return s;
}

template<> inline vec2f from_string(string s) {
    std::regex rgx("[+-]?([0-9]*[.])?[0-9]+");
    std::smatch match;

    if (std::regex_search(s, match, rgx)) {
        expects(match.size() == 3);
        return vec2f{ std::stof(match[1]), std::stof(match[2]) };
    }

    return vec2f{};
}

template<> inline linef from_string(string s) {
    const size_t comma_pos = s.find(",");
    return linef{ from_string<vec2f>(s.substr(0, comma_pos)), from_string<vec2f>(s.substr(comma_pos)) };
}

template<> inline vec2i from_string(string s) {
    std::regex rgx("[+-]?([0-9]*[.])?[0-9]+");
    std::smatch match;

    if (std::regex_search(s, match, rgx)) {
        expects(match.size() == 3);
        return vec2i{ std::stoi(match[1]), std::stoi(match[2]) };
    }

    return vec2i{};
}

template<> inline vec3f from_string(string s) {
    std::regex rgx("[+-]?([0-9]*[.])?[0-9]+");
    std::smatch match;

    if (std::regex_search(s, match, rgx)) {
        expects(match.size() == 4);
        return vec3f{ std::stof(match[1]), std::stof(match[2]), std::stof(match[3]) };
    }

    return vec3f{};
}

template<> inline vec4f from_string(string s) {
    std::regex rgx("[+-]?([0-9]*[.])?[0-9]+");
    std::smatch match;

    if (std::regex_search(s, match, rgx)) {
        expects(match.size() == 5);
        return vec4f{ std::stof(match[1]), std::stof(match[2]), std::stof(match[3]), std::stof(match[4]) };
    }

    return vec4f{};
}