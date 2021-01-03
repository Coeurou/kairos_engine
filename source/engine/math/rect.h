#pragma once

#include <cmath>
#include <regex>
#include <type_traits>

#include <core/contract.h>
#include <core/format.h>
#include <core/result.h>
#include <core/types.h>
#include <math/axis.h>

namespace kairos {

/** Need to comment class to understand the orientation of the rect is top > bottom right > left
 * ???*/
template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>> struct rect {
    using point = vec<2, T>;
    using value_type = T;

    point my_top_left{value_type{}};
    point my_bottom_right{value_type{}};

    inline value_type left() const { return my_top_left.x; }

    inline value_type right() const { return my_bottom_right.x; }

    inline value_type top() const { return my_top_left.y; }

    inline value_type bottom() const { return my_bottom_right.y; }
};

using rectf = rect<float>;

template <class T> sizef size(const rect<T>& rect) { return sizef{width(rect), height(rect)}; }

template <class T> T width(const rect<T>& rect) { return (rect.right() - rect.left()); }

template <class T> T height(const rect<T>& rect) { return (rect.bottom() - rect.top()); }

template <class T> pointf center(const rect<T>& rect) {
    return pointf{rect.left() + (width(rect) * 0.5f), rect.top() + (height(rect) * 0.5f)};
}

template <class T> void translate(rect<T>& rect, const pointf& point) {
    rect.my_top_left.x += static_cast<T>(point.x);
    rect.my_top_left.y += static_cast<T>(point.y);
    rect.my_bottom_right.x += static_cast<T>(point.x);
    rect.my_bottom_right.y += static_cast<T>(point.y);
}

template <class T> bool contains(const rect<T>& rect, const pointf& point) {
    return (point.x >= rect.left() && point.x <= rect.right()) &&
           (point.y <= rect.top() && point.y >= rect.bottom());
}

template <class T> result<rect<T>> intersection(const rect<T>& lhs, const rect<T>& rhs) {
    const T top = std::min(lhs.top(), rhs.top());
    const T left = std::max(lhs.left(), rhs.left());
    const T bottom = std::max(lhs.bottom(), rhs.bottom());
    const T right = std::min(lhs.right(), rhs.right());

    if (top > bottom && right > left) { // sure about top > bottom ?
        return result<rect<T>>{rect<T>::point(top, left), rect<T>::point(bottom, right)};
    } else {
        return result<rect<T>>(error_type::runtime_error);
    }
}

template <class T> void normalize(rect<T>& r) {
    r.my_bottom_right = r.my_top_left + rect<T>::point(static_cast<T>(1));
}

inline string to_string(const rect<int>& r) {
    return format("Rectangle - x: {}, y: {}, width: {}, height: {}", r.my_top_left.x,
                       r.my_top_left.y, width(r), height(r));
}

inline string to_string(const rectf& r) {
    return format("Rectangle - x: {}, y: {}, width: {}, height: {}", r.my_top_left.x,
                       r.my_top_left.y, width(r), height(r));
}

template<class T> inline rect<T> from_string(const string& s) {
    std::regex rgx("[+-]?([0-9]*[.])?[0-9]+");
    std::smatch match;

    std::regex_search(s, match, rgx);
    expects(match.size() == 4, "[rect::from_string] input string is ill formed, couldn't parse it");
    if constexpr (std::is_floating_point_v<T>) {
        return rectf{{std::stof(match[0]), std::stof(match[1])},
            {std::stof(match[2]), std::stof(match[3])}};
    }
    else if constexpr (std::is_integral_v<T>) {
        return rect<int>{{std::stoi(match[0]), std::stoi(match[1])},
            {std::stoi(match[2]), std::stoi(match[3])}};
    }
    else {
        static_assert(false, "Unknown numeric type not compatible with from_string. Please implement it.");
    }

    return rect<T>{};
}

template<class T> inline void flip(rect<T>& r, axis axis) {
    if (axis == axis::x) {
        std::swap(r.my_top_left.x, r.my_bottom_right.x);
    }
    else if (axis == axis::y) {
        std::swap(r.my_top_left.y, r.my_bottom_right.y);
    }
}

} // namespace kairos