#include <graphics/service/detail/video_service_helpers.h>

#include <core/contract.h>
#include <core/format.h>
#include <window/sdl_window.h>
#include <window/window_params.h>

namespace kairos {
namespace detail {

bool init_video_system() {
    expects(SDL_Init(SDL_INIT_VIDEO) >= 0,
            format("Couldn't initilize SDL: {}", SDL_GetError()));
    return true;
}

void cleanup_video_system() { SDL_Quit(); }

window create_window_impl(const window_params& params) {
    return window{ sdl_window(params) };
}

} // namespace detail
} // namespace kairos