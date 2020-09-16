#pragma once

#include <functional>

enum class layer_type {
    gameplay,
    ui,
    debug,
    count
};

struct rendering_layer {
    layer_type my_type{ layer_type::count };
    std::function<void()> my_render_function{};

    void render();
};