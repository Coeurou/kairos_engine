#include <easing_lib.h>
#include <cmath>

static constexpr float c1 = 1.70158f;
static constexpr float c2 = c1 * 1.525f;
static constexpr float c3 = c1 + 1.f;
template <class T> constexpr T pi = T(3.1415926535897932385L);
static constexpr float c4 = (2.f * pi<float>) / 3.f;
static constexpr float c5 = (2.f * pi<float>) / 4.5f;
static constexpr float n1 = 7.5625f;
static constexpr float d1 = 2.75f;

float ease_in_sine(float x) { return 1.f - cosf(x * pi<float> * 0.5f); }

float ease_out_sine(float x) { return sinf((x * pi<float>)*0.5f); }

float ease_in_out_sine(float x) { return -cosf(x * pi<float> - 1.f) * 0.5f; }

float ease_in_quad(float x) { return x * x; }

float ease_out_quad(float x) { return 1.f - (1.f - x) * (1.f - x); }

float ease_in_out_quad(float x) {
    return (x < 0.5f) ? 2.f * x * x : 1.f - (-2.f * x + 2.0f) * (-2.f * x + 2.0f) * 0.5f;
}

float ease_in_cubic(float x) { return x * x * x; }

float ease_out_cubic(float x) { return 1.f - pow(1.f - x, 3); }

float ease_in_out_cubic(float x) {
    return (x < 0.5f) ? 4.f * pow(x, 3) : 1 - pow(-2.f * x + 2.f, 3) * 0.5f;
}

float ease_in_quart(float x) { return pow(x, 4); }

float ease_out_quart(float x) { return 1.f - pow(1.f - x, 4); }

float ease_in_out_quart(float x) {
    return (x < 0.5f) ? 8.f * pow(x, 4) : 1 - pow(-2.f * x + 2.f, 4) * 0.5f;
}

float ease_in_quint(float x) { return pow(x, 5); }

float ease_out_quint(float x) { return 1.f - pow(1.f - x, 5); }

float ease_in_out_quint(float x) {
    return (x < 0.5f) ? 16.f * pow(x, 5) : 1.f - pow(-2.f * x + 2.f, 5) * 0.5f;
}

float ease_in_expo(float x) { return (x == 0.f) ? 0 : pow(2, 10.f * x - 10.f); }

float ease_out_expo(float x) { return (x == 1.f) ? 1 : 1.f - pow(2, -10.f * x); }

float ease_in_out_expo(float x) {
    return (x == 0.f) ? 0.f
                      : (x == 1.f) ? 1.f
                                   : (x < 0.5f) ? pow(2.f, 20.f * x - 10.f) * 0.5f
                                                : (2.f - pow(2.f, -20.f * x + 10.f)) * 0.5f;
}

float ease_in_circ(float x) { return 1.f - sqrtf(1.f - x * x); }

float ease_out_circ(float x) { return sqrtf(1.f - (x - 1.f) * (x - 1.f)); }

float ease_in_out_circ(float x) {
    return (x < 0.5f) ? 1.f - sqrtf(1.f - (2.f * x) * (2.f * x)) * 0.5f
                      : (sqrtf(1.f - pow(-2.f * x + 2.f, 2)) + 1.f) * 0.5f;
}

float ease_in_back(float x) { return c3 * x * x * x - c1 * x * x; }

float ease_out_back(float x) { return 1.f + c3 * pow(x - 1.f, 3) + c1 * pow(x - 1.f, 2); }

float ease_in_out_back(float x) {
    return x < 0.5f ? (pow(2.f * x, 2.f) * ((c2 + 1.f) * 2.f * x - c2)) * 0.5f
                    : (pow(2.f * x - 2.f, 2.f) * ((c2 + 1.f) * (x * 2.f - 2.f) + c2) + 2.f) * 0.5f;
}

float ease_in_elastic(float x) {
    return (x == 0.f)
               ? 0.f
               : x == 1.f ? 1.f : -pow(2.f, 10.f * x - 10.f) * sinf((x * 10.f - 10.75f) * c4);
}

float ease_out_elastic(float x) {
    return (x == 0.f) ? 0.f
                      : x == 1.f ? 1.f : pow(2.f, -10.f * x) * sinf((x * 10.f - 0.75f) * c4) + 1.f;
}

float ease_in_out_elastic(float x) {
    return (x == 0.f)
               ? 0.f
               : x == 1.f
                     ? 1.f
                     : (x < 0.5f)
                           ? -(pow(2.f, 20.f * x - 10.f) * sinf((20.f * x - 11.125f) * c5)) * 0.5f
                           : (pow(2.f, -20.f * x - 10.f) * sinf((20.f * x - 11.125f) * c5)) * 0.5f +
                                 1.f;
}

float ease_in_bounce(float x) { return 1.f - ease_out_bounce(1.f - x); }

float ease_out_bounce(float x) {
    if (x < 1.f / d1) {
        return n1 * x * x;
    } else if (x < 2.f / d1) {
        return n1 * (x - 1.5f / d1) * x + 0.75f;
    } else if (x < 2.5f / d1) {
        return n1 * (x - 2.25f / d1) * x + 0.9375f;
    } else {
        return n1 * (x - 2.625f / d1) * x + 0.984375f;
    }
}

float ease_in_out_bounce(float x) {
    return (x < 0.5f) ? (1.f - ease_out_bounce(1.f - 2.f * x)) * 0.5f
                      : (1.f + ease_out_bounce(2.f * x - 1.f)) * 0.5f;
}
