#pragma once

#include <core/types.h>
#include <SDL.h>

namespace kairos {

class opengl_context {
  public:
    enum class opengl_profile { core, compat, es };

    uint8 my_major_version{3};
    uint8 my_minor_version{3};
    uint8 my_aa_samples{4};
    opengl_profile my_profile{opengl_profile::core};

    int get_version() const { return my_major_version * 100 + my_minor_version * 10; }

    virtual ~opengl_context() {}
    virtual void setup_gl_context() = 0;
    virtual void setup_gl_functions() const = 0;
    virtual void swap_buffers() const = 0;
};

class sdl_window;
class window;

class sdl_opengl_context : public opengl_context {
  public:
    sdl_opengl_context(const window& window);
    ~sdl_opengl_context();

    void setup_gl_context() override;
    void setup_gl_functions() const override;
    void swap_buffers() const override;

    const window& my_window;
    SDL_GLContext my_sdl_gl_context{};
};

void setup_default_gl_rendering_options();

} // namespace kairos