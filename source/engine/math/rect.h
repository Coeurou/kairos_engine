#pragma once

#include <cmath>
#include <type_traits>

#include <core/formattable.h>
#include <core/result.h>

namespace kairos {

/** Need to comment class to understand the orientation of the rect is top > bottom right > left
 * ???*/
template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>> struct rect {
    glm::vec<2, T, glm::defaultp> my_top_left{T{}};
    glm::vec<2, T, glm::defaultp> my_bottom_right{T{}};

    inline T left() const { return my_top_left.x; }

    inline T right() const { return my_bottom_right.x; }

    inline T top() const { return my_top_left.y; }

    inline T bottom() const { return my_bottom_right.y; }
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
        return result<rect<T>>{{top, left} {bottom, right}};
    } else {
        return result<rect<T>>(error_type::runtime_error);
    }
}

template <class T> void normalize(rect<T>& rect) {
    rect.my_bottom_right = my_top_left + glm::vec<2, T, glm::defaultp>{static_cast<T>(1)};
}

template <> inline string to_string(rect<int> r) {
    return fmt::format("Rectangle - x: {}, y: {}, width: {}, height: {}", r.my_top_left.x,
                       r.my_top_left.y, width(r), height(r));
}

template <> inline string to_string(const rect<int>& r) {
    return fmt::format("Rectangle - x: {}, y: {}, width: {}, height: {}", r.my_top_left.x,
                       r.my_top_left.y, width(r), height(r));
}

template <> inline string to_string(rectf r) {
    return fmt::format("Rectangle - x: {}, y: {}, width: {}, height: {}", r.my_top_left.x,
                       r.my_top_left.y, width(r), height(r));
}

template <> inline string to_string(const rectf& r) {
    return fmt::format("Rectangle - x: {}, y: {}, width: {}, height: {}", r.my_top_left.x,
                       r.my_top_left.y, width(r), height(r));
}

template <> inline rectf from_string(string s) {
    std::regex rgx("[+-]?([0-9]*[.])?[0-9]+");
    std::smatch match;

    if (std::regex_search(s, match, rgx)) {
        expects(match.size() == 4);
        return rectf{{std::stof(match[0]), std::stof(match[1])},
                     {std::stof(match[2]), std::stof(match[3])}};
    }

    return rectf{};
}

template <> inline rect<int> from_string(string s) {
    std::regex rgx("[+-]?([0-9]*[.])?[0-9]+");
    std::smatch match;

    if (std::regex_search(s, match, rgx)) {
        expects(match.size() == 4);
        return rect<int>{{std::stoi(match[0]), std::stoi(match[1])},
                         {std::stoi(match[2]), std::stoi(match[3])}};
    }

    return rect<int>{};
}

} // namespace kairos