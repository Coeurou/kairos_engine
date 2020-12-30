#pragma once

float ease_in_sine(float x);

float ease_out_sine(float x);

float ease_in_out_sine(float x);

float ease_in_quad(float x);

float ease_out_quad(float x);

float ease_in_out_quad(float x);

float ease_in_cubic(float x);

float ease_out_cubic(float x);

float ease_in_out_cubic(float x);

float ease_in_quart(float x);

float ease_out_quart(float x);

float ease_in_out_quart(float x);

float ease_in_quint(float x);

float ease_out_quint(float x);

float ease_in_out_quint(float x);

float ease_in_expo(float x);

float ease_out_expo(float x);

float ease_in_out_expo(float x);

float ease_in_circ(float x);

float ease_out_circ(float x);

float ease_in_out_circ(float x);

float ease_in_back(float x);

float ease_out_back(float x);

float ease_in_out_back(float x);

float ease_in_elastic(float x);

float ease_out_elastic(float x);

float ease_in_out_elastic(float x);

float ease_in_bounce(float x);

float ease_out_bounce(float x);

float ease_in_out_bounce(float x);

// a+t(b-a) => start + t * (end - start)
template <class T> T lerp(T start, T end, float t) {
    return start + t * (end - start);
}