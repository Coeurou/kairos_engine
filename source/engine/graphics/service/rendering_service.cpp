#include <graphics/service/rendering_service.h>
#include <graphics/service/detail/rendering_service_helpers.h>

namespace kairos {

result<opengl_context*> rendering_service::create_context(const gl_params& params,
                                                          const window& window) {
    if (my_gl_context = detail::create_context(window)) {
        my_gl_context->my_major_version = params.my_gl_version[0];
        my_gl_context->my_minor_version = params.my_gl_version[1];
        my_gl_context->my_aa_samples = params.my_aa_samples;
        my_gl_context->my_profile = params.my_gl_profile;
        my_gl_context->setup_gl_context();
        my_gl_context->setup_gl_functions();
        setup_default_gl_rendering_options();
    } else {
        return result<opengl_context*>(error_type::invalid_result);
    }

    my_graphics_impl = (my_gl_context->my_profile == opengl_context::opengl_profile::es)
                           ? renderer_engine::opengl_es
                           : renderer_engine::opengl;

    return result<opengl_context*>(my_gl_context.get());
}

void rendering_service::swap_buffers() const { my_gl_context->swap_buffers(); }

void rendering_service::set_pixel_size(const sizef& pixel_size) { my_pixel_size = pixel_size; }

sizef rendering_service::pixel_size() const { return my_pixel_size; }

} // namespace kairos