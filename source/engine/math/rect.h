#pragma once

#include <cmath>
#include <type_traits>
#include <formattable.h>

template<typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
struct rect {
    glm::vec<2, T, glm::defaultp> my_top_left{ T{} };
    glm::vec<2, T, glm::defaultp> my_bottom_right{ T{} };

    inline pointf center() const {
        return my_top_left + pointf{ (my_bottom_right.x - my_top_left.x) * 0.5f, (my_top_left.y - my_bottom_right.y) * 0.5f };
    }

    inline sizef size() const {
        return sizef{ width(), height() };
    }

    inline T width() const {
        return (my_bottom_right.x - my_top_left.x);
    }

    inline T height() const {
        return (my_top_left.y - my_bottom_right.y);
    }
    
    inline T left() const {
        return my_top_left.x;
    }

    inline T right() const {
        return my_bottom_right.x;
    }

    inline T top() const {
        return my_top_left.y;
    }

    inline T bottom() const {
        return my_bottom_right.y;
    }

    inline bool contains(pointf point) const {
        return (point.x >= my_top_left.x && point.x <= my_bottom_right.x) && (point.y <= my_top_left.y && point.y >= my_bottom_right.y)
    }

    inline void intersect(const rect& rectangle) {
        const T top = std::min(my_top_left.y, rectangle.my_top_left.y);
        const T left = std::max(my_top_left.x, rectangle.my_top_left.x);
        const T bottom = std::max(my_bottom_right.y, rectangle.my_bottom_right.x);
        const T right = std::min(my_bottom_right.x, rectangle.my_bottom_right.x);

        if (top > bottom && right > left) {
            my_top_left = { top, left };
            my_bottom_right = { bottom, right };
        }
    }

    inline rect intersected(const rect& rectangle) const {
        rect res = *this;
        res.intersect(rectangle);
        return res;
    }

    inline void normalize() {
        my_bottom_right = my_top_left + glm::vec<2, T, glm::defaultp>{static_cast<T>(1)};
    }

    inline rect normalized() const {
        rect res = *this;
        res.normalize();
        return res;
    }

    inline void translate(pointf point) {
        my_top_left += point;
        my_bottom_right += point;
    }

    inline rect translated(pointf point) const {
        rect res = *this;
        res.translate(point);
        return res;
    }
};

using rectf = rect<float>;

template<> inline string to_string(rectf r) {
    return fmt::format("Rectangle - x: {}, y: {}, width: {}, height: {}", r.my_top_left.x, r.my_top_left.y, r.width(), r.height());
}