#pragma once

#include <SDL.h>
#include <core/types.h>
#include <graphics/opengl/opengl_context.h>

namespace kairos {

namespace {
static const dictionary<SDL_GLattr, gl_attribute> our_sdl_gl_attribute_adapter = {
    {SDL_GL_RED_SIZE, gl_attribute::red_size},
    {SDL_GL_GREEN_SIZE, gl_attribute::green_size},
    {SDL_GL_BLUE_SIZE, gl_attribute::blue_size},
    {SDL_GL_ALPHA_SIZE, gl_attribute::alpha_size},
    {SDL_GL_BUFFER_SIZE, gl_attribute::framebuffer_size},
    {SDL_GL_DOUBLEBUFFER, gl_attribute::double_buffering},
    {SDL_GL_DEPTH_SIZE, gl_attribute::depth_size},
    {SDL_GL_STENCIL_SIZE, gl_attribute::stencil_size},
    {SDL_GL_ACCUM_RED_SIZE, gl_attribute::accum_red_size},
    {SDL_GL_ACCUM_GREEN_SIZE, gl_attribute::accum_green_size},
    {SDL_GL_ACCUM_BLUE_SIZE, gl_attribute::accum_blue_size},
    {SDL_GL_ACCUM_ALPHA_SIZE, gl_attribute::accum_alpha_size},
    {SDL_GL_STEREO, gl_attribute::stereo},
    {SDL_GL_MULTISAMPLEBUFFERS, gl_attribute::multisample_buffers},
    {SDL_GL_MULTISAMPLESAMPLES, gl_attribute::aa_samples},
    {SDL_GL_ACCELERATED_VISUAL, gl_attribute::hardware_acc},
    {SDL_GL_CONTEXT_MAJOR_VERSION, gl_attribute::major_version},
    {SDL_GL_CONTEXT_MINOR_VERSION, gl_attribute::minor_version},
    {SDL_GL_CONTEXT_FLAGS, gl_attribute::count},
    {SDL_GL_CONTEXT_PROFILE_MASK, gl_attribute::context_profile},
    {SDL_GL_SHARE_WITH_CURRENT_CONTEXT, gl_attribute::count},
    {SDL_GL_FRAMEBUFFER_SRGB_CAPABLE, gl_attribute::count},
    {SDL_GL_CONTEXT_RELEASE_BEHAVIOR, gl_attribute::count}};

static const dictionary<opengl_context::opengl_profile, SDL_GLprofile>
    our_sdl_gl_profile_adapter = {
        {opengl_context::opengl_profile::core, SDL_GLprofile::SDL_GL_CONTEXT_PROFILE_CORE},
        {opengl_context::opengl_profile::compat,
         SDL_GLprofile::SDL_GL_CONTEXT_PROFILE_COMPATIBILITY},
        {opengl_context::opengl_profile::es, SDL_GLprofile::SDL_GL_CONTEXT_PROFILE_ES}};
} // namespace

class sdl_opengl_context {
  public:
    sdl_opengl_context(int major, int minor);

    uint32 get_window_id() const;
    void set_window_id(uint32 window_id);

    SDL_GLContext get_sdl_gl_context() const;
    void set_sdl_gl_context(SDL_GLContext context);

  private:
      SDL_GLContext my_sdl_gl_context{};
      uint32 my_window_id = 0;
};

bool setup(sdl_opengl_context& gl_context, uint32 window_id);
void swap_buffers(sdl_opengl_context& gl_context);
int version(const sdl_opengl_context& gl_context);
int get_attribute(const sdl_opengl_context& gl_context, gl_attribute attr);
void set_attribute(sdl_opengl_context& gl_context, gl_attribute attr, int value);
void destroy(sdl_opengl_context& gl_context);

} // namespace kairos