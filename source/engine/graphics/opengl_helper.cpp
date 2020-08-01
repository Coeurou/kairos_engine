#pragma once

#include <opengl_helper.h>

void clear_color(const vec4f& color) {
    glClearColor(color.r, color.g, color.b, color.a);
    glClear(GL_COLOR_BUFFER_BIT);
}