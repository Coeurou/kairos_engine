#pragma once

#include <optional>

#include <types.h>
#include <buffer_object.h>

class vao {
public:
    uint32 my_gl_id{ 0 };
    string my_name{ "" };
    bool is_bound{ false };

    void bind() const;
    void unbind() const;
    void set_vertex_buffer(const buffer_object& vbo) const;
    void set_index_buffer(const buffer_object& ibo) const;

    static constexpr uint32 position_attrib_index = 0;
    static constexpr uint32 color_attrib_index = 1;
    static constexpr uint32 texcoords_attrib_index = 2;
    static constexpr uint32 tex_id_attrib_index = 3;

    static void setup(vao& vao);
    static std::optional<vao> find(string_view name);

private:
    void set_buffer(const buffer_object& buffer) const;
};

inline array<vao> our_vaos;