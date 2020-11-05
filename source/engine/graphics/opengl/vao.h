#pragma once

#include <core/types.h>
#include <graphics/opengl/buffer_object.h>

class vao {
public:
    uint32 my_gl_id{ 0 };
    string my_name{ "" };

    vao();

    vao(const vao& other) = delete;
    vao& operator=(const vao& other) = delete;

    vao(vao&& other) noexcept;
    vao& operator=(vao&& other) noexcept;

    void bind() const;
    void unbind() const;

    static constexpr uint32 position_attrib_index = 0;
    static constexpr uint32 color_attrib_index = 1;
    static constexpr uint32 texcoords_attrib_index = 2;
    static constexpr uint32 tex_id_attrib_index = 3;

    void setup() const;
    void cleanup();
    static vao* find(string_view name);
};

inline bool operator==(const vao& lhs, const vao& rhs) {
    return lhs.my_gl_id == rhs.my_gl_id && lhs.my_name == rhs.my_name;
}

inline array<vao> our_vaos;