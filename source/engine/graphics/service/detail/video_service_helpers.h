#pragma once

#include <memory>
#include <window/window.h>

namespace kairos {
namespace detail {

bool init_video_system();

void cleanup_video_system();

window create_window_impl(const window_params& params);

} // namespace detail
} // namespace kairos