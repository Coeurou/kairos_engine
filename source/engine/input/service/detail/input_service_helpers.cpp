#include <input/service/detail/input_service_helpers.h>
#include <SDL.h>

namespace kairos {

namespace detail {

bool enable(input_device device) {
    if (device == input_device::controller) {
        return SDL_InitSubSystem(SDL_INIT_GAMECONTROLLER) > 0;
    }
    return false;
}

} // namespace detail

} // namespace kairos
