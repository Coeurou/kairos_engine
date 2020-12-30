#pragma once


#include <core/logger.h>
#include <core/types.h>
#include <graphics/vertex.h>

enum class buffer_target {
    geometry,
    indices,
    count
};

class buffer_object {
public:
    uint32 my_gl_id{ 0 };
    GLenum my_usage{ GL_STATIC_DRAW };
    buffer_target my_target{ buffer_target::count };
    uint32 my_size{ 0 };

    buffer_object() = default;

    buffer_object(const buffer_object&) = delete;
    buffer_object& operator=(const buffer_object&) = delete;

    buffer_object(buffer_object&& other) noexcept;
    buffer_object& operator=(buffer_object&& other) noexcept;

    template<typename container>
    void setup(const container& data) {
        glGenBuffers(1, &my_gl_id);

        GLenum gl_target = as_gl_target(my_target);

        glBindBuffer(gl_target, my_gl_id);
        using data_container = typename std::remove_reference_t<container>;
        using data_type = typename data_container::value_type;
        my_size = static_cast<uint32>(count(data));
        glBufferData(gl_target, my_size * sizeof(data_type), data.data(), my_usage);
    }

    void bind() const;
    void unbind() const;
    void cleanup();

    static GLenum as_gl_target(buffer_target target);
};