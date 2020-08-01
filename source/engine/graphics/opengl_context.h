#pragma once

#include <types.h>

class opengl_context {
public:
    enum class opengl_profile {
        core,
        compat
    };

    uint8 major_version{ 3 };
    uint8 minor_version{ 3 };
    uint8 aa_samples{ 4 };
    opengl_profile profile{ opengl_profile::core };

    void setup_gl_context() const;
    void setup_gl_functions() const;
    void swap_buffers() const;
};

void setup_default_gl_rendering_options();