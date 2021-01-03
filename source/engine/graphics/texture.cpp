#pragma once

#include <graphics/texture.h>

#include <glad/glad.h>
#include <nameof.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include <graphics/stb_image.h>

#include <core/algorithms.h>
#include <core/formattable.h>

namespace kairos {

static_array<uint32, texture::our_limit> texture::our_textures_idx = {0};

/** Member functions */

texture::texture() : my_data(new texture_data()) { ++my_data->my_ref_count; }

texture::~texture() {
    if (my_data != nullptr && --my_data->my_ref_count <= 0) {
        auto available_unit = find(texture::our_textures_idx, my_data->my_id);
        (*available_unit) = 0;
        glDeleteTextures(1, &my_data->my_id);
        delete my_data;
    }
}

texture::texture(const texture& other) {
    this->~texture();
    my_data = other.my_data;
    ++my_data->my_ref_count;
}

texture::texture(texture&& other) noexcept {
    this->~texture();
    my_data = std::exchange(other.my_data, nullptr);
}

texture& texture::operator=(const texture& other) {
    this->~texture();
    my_data = other.my_data;
    ++my_data->my_ref_count;
    return *this;
}

texture& texture::operator=(texture&& other) noexcept {
    my_data = std::exchange(other.my_data, nullptr);
    return *this;
}

uint32 texture::id() const { return my_data->my_id; }

int texture::index() const { return my_data->my_index; }

int texture::channels() const { return my_data->my_channels; }

vec2i texture::size() const { return my_data->my_size; }

/** Non-member functions */

uint32 id(const texture& texture) { return texture.id(); }

vec2i size(const texture& texture) { return texture.size(); }

void bind(const texture& texture) {
    glActiveTexture(GL_TEXTURE0 + texture.index());
    glBindTexture(GL_TEXTURE_2D, id(texture));
}

void unbind(const texture& /*texture*/) { glBindTexture(GL_TEXTURE_2D, 0); }

result<texture> load(const path& img_path) {
    texture res;

    if (filesystem::exists(img_path)) {
        const auto available_unit = find(texture::our_textures_idx, uint32{0});
        expects(available_unit != texture::our_textures_idx.end(),
                "Couldn't find an available texture unit, consider freeing some before adding a "
                "new one.");
        res.my_data->my_index =
            static_cast<uint32>(std::distance(texture::our_textures_idx.begin(), available_unit));

        unsigned char* image = stbi_load(img_path.string().c_str(), &res.my_data->my_size.x,
                                         &res.my_data->my_size.y, &res.my_data->my_channels, 0);
        {
            ensures(image != nullptr,
                    fmt::format("Couldn't load texture image: {}", img_path.string()));

            glGenTextures(1, &res.my_data->my_id);
            texture::our_textures_idx[res.my_data->my_index] = res.my_data->my_id;
            glBindTexture(GL_TEXTURE_2D, res.my_data->my_id);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, res.my_data->my_size.x, res.my_data->my_size.y,
                         0, GL_RGBA, GL_UNSIGNED_BYTE, image);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        stbi_image_free(image);

        return result<texture>(res);
    } else {
        return result<texture>(error_type::invalid_parameter);
    }
}

rect<int> bounds(const texture& texture) { return rect<int>{{0, 0}, size(texture)}; }

string to_string(const texture& t) {
    return fmt::format("Texture: {}, {}, {}", id(t), to_string(size(t)), t.channels());
}

} // namespace kairos