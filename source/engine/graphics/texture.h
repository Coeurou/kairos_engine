#pragma once

#include <core/filesystem.h>
#include <core/result.h>
#include <math/rect.h>

namespace kairos {

class texture {
    friend result<texture> load(const path& img_path);

  public:
    /** Member functions */

    texture();
    ~texture();
    texture(const texture& other);
    texture(texture&& other) noexcept;
    texture& operator=(const texture& other);
    texture& operator=(texture&& other) noexcept;

    uint32 id() const;
    int index() const;
    int channels() const;
    vec2i size() const;

    static constexpr uint32 our_limit = 8;

private:
    static static_array<uint32, our_limit> our_textures_idx;

    struct texture_data {
        uint32 my_id{0};
        int my_index{-1};
        int my_channels{0};
        vec2i my_size{0, 0};
        int my_ref_count;
    };
    texture_data* my_data = nullptr;
};

/** Non-member functions */

uint32 id(const texture& texture);

vec2i size(const texture& texture);

void bind(const texture& texture);

void unbind(const texture& texture);

/** Load a texture from file, return an invalid parameter error if img_path doesn't exist. */
result<texture> load(const path& img_path);

rect<int> bounds(const texture& texture);

string to_string(const texture& t);

} // namespace kairos