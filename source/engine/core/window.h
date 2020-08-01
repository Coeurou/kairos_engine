#pragma once

#include <rendering_layer.h>
#include <types.h>

class window {
public:
    vec2i size() const;
    void render();

    native_window* native_window;
    static_array<rendering_layer, static_cast<size_t>(layer_type::count)> layers; // should be in a canvas class exportable to image (file or texture)
};

window create_window(int width, int height, string_view title);