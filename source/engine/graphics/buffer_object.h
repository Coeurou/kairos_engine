#pragma once


#include <logger.h>
#include <types.h>
#include <vertex.h>

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

    template<typename buffer_data>
    static void setup(buffer_object& buffer, const array<buffer_data>& data) {
        glGenBuffers(1, &buffer.my_gl_id);

        GLenum gl_target = as_gl_target(buffer.my_target);

        glBindBuffer(gl_target, buffer.my_gl_id);
        glBufferData(gl_target, sizeof(data), data.data(), buffer.my_usage);
        buffer.my_size = data.size();
    }

    void bind() const;
    void cleanup();

private:
    static GLenum as_gl_target(buffer_target target);
};