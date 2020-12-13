#include <graphics/service/video_service.h>
#include <graphics/service/detail/video_service_helpers.h>

namespace kairos {

result<uint32> video_service::setup() {
    if (detail::init_video_system()) {
        return result<uint32>(EXIT_SUCCESS);
    } else {
        return result<uint32>(error_type::invalid_result);
    }
}

result<uint32> video_service::cleanup() {
    detail::cleanup_video_system();
    return result<uint32>(EXIT_SUCCESS);
}

} // namespace kairos