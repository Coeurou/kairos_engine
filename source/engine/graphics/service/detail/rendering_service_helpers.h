#pragma once

#include <memory>
#include <graphics/opengl/opengl_context.h>
#include <window/window.h>

namespace kairos {
namespace detail {

std::unique_ptr<opengl_context> create_context(const window& window);

} // namespace detail
} // namespace kairos
