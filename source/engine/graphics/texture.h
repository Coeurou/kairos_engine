#pragma once

#include <filesystem.h>
#include <formattable.h>
#include <core_application.h>

class texture;

class texture_impl {
  public:
    virtual void setup(texture& t, const path& img_path) = 0;
    virtual void bind(uint32 index, uint32 texture_id) = 0;
    virtual void cleanup(texture& t) = 0;
};

class opengl_texture : public texture_impl {
  public:
    void setup(texture& t, const path& img_path) override;
    void bind(uint32 index, uint32 texture_id) override;
    void cleanup(texture& t) override;

    static uint32 get_texture_units_count();
};

class texture {
public:
    uint32 my_id{0};
    int my_index{-1};
    int my_channels{0};
    vec2i my_size{0, 0};

    static std::shared_ptr<texture_impl> our_impl;
    static constexpr uint32 our_limit = 8;
    static static_array<uint32, our_limit> our_textures_idx;

    void setup(const path& img_path);
    void bind();
    void unbind();
    void cleanup();

    static void set_implementation(renderer_engine backend);
};

template <> inline string to_string(texture t) {
    return fmt::format("Texture: {}, {}, {}", t.my_id, to_string(t.my_size), t.my_channels);
}