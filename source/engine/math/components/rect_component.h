#pragma once

#include <core/component.h>
#include <math/rect.h>

struct rect_component : public component {
    rectf my_geometry{{0.f, 0.f}, {0.f, 0.f}};
    rect_component(uint32 id, rectf rect) : component(id), my_geometry(rect) {}
};