#pragma once

#include <functional>

#include <core/result.h>
#include <core/types.h>

namespace {

struct main_loop {
    array<std::function<void()>> my_start_frame_functions;
    array<std::function<void()>> my_end_frame_functions;
    array<std::function<result<uint32>()>> my_update_functions;
    array<std::function<result<uint32>()>> my_render_functions;

    result<uint32> run();
};

} // namespace;

class game {
public:
    bool setup() { return true; }
    void run();
    void cleanup() {}
    void add_start_frame_function(std::function<void()>&& f);
    void add_end_frame_function(std::function<void()>&& f);
    void add_update_function(std::function<result<uint32>()>&& f);
    void add_render_function(std::function<result<uint32>()>&& f);

private:
    main_loop my_main_loop;
};