#pragma once

#include <graphics/painter.h>
#include "painter.h"

namespace kairos {
/** Member functions */

painter::painter(const painter& p) {
    this->~painter();
    my_self = p.my_self;
    my_self->ref_count++;
}

painter& painter::operator=(const painter& p) {
    this->~painter();
    my_self = p.my_self;
    my_self->ref_count++;

    return *this;
}

painter::painter(painter&& p) noexcept { 
    this->~painter();
    my_self = std::exchange(p.my_self, nullptr);
}

painter& painter::operator=(painter&& p) noexcept {
    this->~painter();
    my_self = std::exchange(p.my_self, nullptr);

    return *this;
}

painter::~painter() {
    if (my_self != nullptr && --my_self->ref_count <= 0) {
        delete my_self;
        my_self = nullptr;
    }
}

void clear(const painter& painter, const color& color) { painter.my_self->clear(color); }

void draw(painter& painter, const sprite& sprite) {
    painter.my_self->draw(painter::drawable::sprite, &sprite, 1);
}

void draw(painter& painter, const array<sprite>& sprites) {
    painter.my_self->draw(painter::drawable::sprite, &sprites, sprites.size());
}

void draw(painter& painter, const rectf& rect) {
    painter.my_self->draw(painter::drawable::rectangle, &rect, 1);
}

void draw(painter& painter, const array<rectf>& rects) {
    painter.my_self->draw(painter::drawable::rectangle, &rects, rects.size());
}

void draw(painter& painter, const linef& line) {
    painter.my_self->draw(painter::drawable::line, &line, 1);
}

void draw(painter& painter, const array<linef>& lines) {
    painter.my_self->draw(painter::drawable::line, &lines, lines.size());
}

} // namespace kairos