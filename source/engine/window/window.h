#pragma once

#include <core/formattable.h>
#include <core/types.h>

class window_interface {
public:
    virtual ~window_interface() {}

    virtual vec2i size() const = 0;
    virtual void set_size(vec2i size) = 0;
    virtual void start_frame() = 0;
    virtual void end_frame() = 0;
    virtual bool should_close() const = 0;
    virtual void set_should_close(bool should_close) = 0;

    virtual void window_resized() = 0;
};

struct window_params {
    static constexpr float our_undefined_pos = -1.f;
    string my_title{ "" };
    sizef my_size{ 0.f, 0.f };
    pointf my_pos{ our_undefined_pos, our_undefined_pos };
};

inline string to_string(const window_params& params) {
    return fmt::format("size: {}, pos: {}, title: {}", to_string(params.my_size), to_string(params.my_pos), params.my_title);
}

template<typename window> std::unique_ptr<window> create_window(const window_params& params) {
    return window::create(params);
}

template<typename graphics> void create_graphics_context() {
    return graphics::create_graphics_context();
}
