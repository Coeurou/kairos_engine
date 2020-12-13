#pragma once

#include <core/renderer_engine.h>
#include <core/result.h>
#include <core/service.h>
#include <graphics/opengl/opengl_context.h>

namespace kairos {

using gl_context_ptr = std::unique_ptr<opengl_context>;

struct gl_params {
    static_array<uint8, 2> my_gl_version{0, 0};
    uint8 my_aa_samples{0};
    opengl_context::opengl_profile my_gl_profile{opengl_context::opengl_profile::core};
};

class rendering_service : public service {
  public:
    result<opengl_context*> create_context(const gl_params& params, const window& window);
    void swap_buffers() const;
    void set_pixel_size(const sizef& pixel_size);
    sizef pixel_size() const;

  private:
    gl_context_ptr my_gl_context;
    renderer_engine my_graphics_impl;
    sizef my_pixel_size{1.f, 1.f};
};

} // namespace kairos