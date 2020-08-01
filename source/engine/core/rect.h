#pragma once

#include <cmath>
#include <type_traits>
#include <types.h>

template<typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
struct rect {
    glm::vec<2, T, glm::defaultp> top_left{ T{} };
    glm::vec<2, T, glm::defaultp> bottom_right{ T{} };

    inline pointf center() const {
        return top_left + pointf{ (bottom_right.x - top_left.x) * 0.5f, (top_left.y - bottom_right.y) * 0.5f };
    }

    inline sizef size() const {
        return sizef{ width(), height() };
    }

    inline T width() const {
        return (bottom_right.x - top_left.x);
    }

    inline T height() const {
        return (top_left.y - bottom_right.y);
    }

    inline bool contains(pointf point) const {
        return (point.x >= top_left.x && point.x <= bottom_right.x) && (point.y <= top_left.y && point.y >= bottom_right.y)
    }

    inline void intersect(const rect& rectangle) {
        const T top = std::min(top_left.y, rectangle.top_left.y);
        const T left = std::max(top_left.x, rectangle.top_left.x);
        const T bottom = std::max(bottom_right.y, rectangle.bottom_right.x);
        const T right = std::min(bottom_right.x, rectangle.bottom_right.x);

        if (top > bottom && right > left) {
            top_left = { top, left };
            bottom_right = { bottom, right };
        }
    }

    inline rect intersected(const rect& rectangle) const {
        rect res = *this;
        res.intersect(rectangle);
        return res;
    }

    inline void normalize() {
        bottom_right = top_left + glm::vec<2, T, glm::defaultp>{static_cast<T>(1)};
    }

    inline rect normalized() const {
        rect res = *this;
        res.normalize();
        return res;
    }

    inline void translate(pointf point) {
        top_left += point;
        bottom_right += point;
    }

    inline rect translated(pointf point) const {
        rect res = *this;
        res.translate(point);
        return res;
    }
};

using rectf = rect<float>;