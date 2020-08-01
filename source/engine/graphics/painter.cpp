#pragma once

#include <painter.h>

void painter::draw_line(const linef& line) {

}

void painter::draw_lines(const array<linef>& lines) {

}

void painter::draw_lines(const array<std::pair<linef, color>>& colored_lines) {

}

void painter::draw_rect(const rectf& rect) {

}

void painter::draw_rects(const array<rectf>& rect) {

}

void painter::draw_rects(const array<std::pair<rectf, color>>& colored_rects) {

}

void painter::draw_rects(const array<std::pair<rectf, texture>>& textured_rects) {

}

void painter::draw_ellipse(pointf center, float rx, float ry) {

}

void painter::draw_ellipse(const rectf& rect) {
    draw_ellipse(rect.center(), rect.width(), rect.height());
}

void painter::draw_ellipses(const array<rectf>& rect) {

}

void painter::draw_ellipses(const array<std::pair<rectf, color>>& colored_ellipses) {

}

void painter::draw_ellipses(const array<std::pair<rectf, texture>>& textured_ellipses) {

}

void painter::draw_text(pointf pos, string_view text) {

}
