#pragma once

#include <types.h>

struct program {
    static constexpr uint32 max_shaders = 4;

    uint32 my_gl_id{ 0 };
    static_array<uint32, max_shaders> my_shaders = {0};
    string my_name{ "" };

    ~program();
    bool setup(std::initializer_list<uint32> shaders);
    bool link();
    void cleanup();
};

inline array<program> our_programs;
