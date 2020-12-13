#include <graphics/service/detail/rendering_service_helpers.h>

namespace kairos {
namespace detail {

std::unique_ptr<opengl_context> create_context(const window& window) {
    return std::make_unique<sdl_opengl_context>(window);
}

}; // namespace detail
} // namespace kairos