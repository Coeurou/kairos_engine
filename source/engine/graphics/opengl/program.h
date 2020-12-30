#pragma once

#include <core/types.h>

namespace kairos {

struct program {
    static constexpr uint32 max_shaders = 4;

    uint32 my_gl_id{0};
    static_array<uint32, max_shaders> my_shaders = {0};
    string my_name{""};

    program() = default;
    program(const program& other) = delete;
    program& operator=(const program& other) = delete;

    program(program&& other) noexcept;
    program& operator=(program&& other) noexcept;

    ~program();

    operator uint32() const { return my_gl_id; }

    bool setup(std::initializer_list<uint32> shaders);
    bool link() const;
    void cleanup();
};

inline array<program> our_programs;

} // namespace kairos