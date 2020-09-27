#pragma once

#include <texture.h>

#include <nameof.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <kairos_lib.h>

static_array<uint32, texture::our_limit> texture::our_textures_idx = {0};
std::shared_ptr<texture_impl> texture::our_impl = nullptr;

void opengl_texture::setup(texture& t, const path& img_path) {
    const auto available_unit = find(texture::our_textures_idx, 0);
    expects(available_unit != texture::our_textures_idx.end(), "Couldn't find an available texture unit, consider freeing some before adding a new one.");
    t.my_index = std::distance(texture::our_textures_idx.begin(), available_unit);

    unsigned char* image = stbi_load(img_path.string().c_str(), &t.my_size.x, &t.my_size.y, &t.my_channels, 0);
    ensures(image != nullptr, fmt::format("Couldn't load texture image: {}", img_path.string()));
    stbi_set_flip_vertically_on_load(true);

    glGenTextures(1, &t.my_id);
    texture::our_textures_idx[t.my_index] = t.my_id;
    glBindTexture(GL_TEXTURE_2D, t.my_id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, t.my_size.x, t.my_size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE,
                 image);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(image);
}

void opengl_texture::bind(uint32 index, uint32 texture_id) {
    glActiveTexture(GL_TEXTURE0 + index);
    glBindTexture(GL_TEXTURE_2D, texture_id);
}

void opengl_texture::cleanup(texture& t) {
    auto available_unit = find(texture::our_textures_idx, t.my_id);
    (*available_unit) = 0;
    glDeleteTextures(1, &t.my_id);
}

void texture::setup(const path& img_path) {
    ensures(our_impl != nullptr, "Texture backend not initialized");
    our_impl->setup(*this, img_path);
}

void texture::bind() {
    ensures(our_impl != nullptr, "Texture backend not initialized");
    our_impl->bind(my_index, my_id);
}

void texture::unbind() {
    ensures(our_impl != nullptr, "Texture backend not initialized");
    our_impl->bind(my_index, 0);
}

void texture::cleanup() {
    ensures(our_impl != nullptr, "Texture backend not initialized");
    unbind();
    our_impl->cleanup(*this);
    my_id = 0;
    my_index = 0;
    my_channels = 0;
    my_size = {0, 0};
}

void texture::set_implementation(renderer_engine backend) {
    switch (backend) {
    case renderer_engine::opengl_es:
    case renderer_engine::opengl: {
        our_impl = std::make_shared<opengl_texture>();
        break;
    }
    default: {
        ensures(false,
                fmt::format("Unimplemented rendering backend, you shouldn't enter here: {}\n", NAMEOF_ENUM(backend)).c_str());
        break;
    }
    }
}
