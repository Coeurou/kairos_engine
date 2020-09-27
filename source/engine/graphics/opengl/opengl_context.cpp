#pragma once

#include <opengl_context.h>

#include <application.h>
#include <contract.h>

sdl_opengl_context::sdl_opengl_context(const window_interface& window) {
    expects(core_application::is_initialized(implementation_type::SDL), "Cannot create a SDL OpenGL context, if SDL is not initialized");
    my_window = &(static_cast<const sdl_window&>(window));
}

sdl_opengl_context::~sdl_opengl_context() {
    SDL_GL_DeleteContext(my_sdl_gl_context);
    my_window = nullptr;
}

void sdl_opengl_context::setup_gl_context() {
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, my_major_version);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, my_minor_version);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, my_aa_samples);

    SDL_GLprofile context_profile = SDL_GL_CONTEXT_PROFILE_CORE;
    if (my_profile == opengl_profile::compat) {
        context_profile = SDL_GL_CONTEXT_PROFILE_COMPATIBILITY;
    }
    else if (my_profile == opengl_profile::es) {
        context_profile = SDL_GL_CONTEXT_PROFILE_ES;
    }
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, context_profile);

    my_sdl_gl_context = SDL_GL_CreateContext(my_window->my_native_window);

    SDL_GL_SetSwapInterval(1); // Enable vsync
    SDL_GL_MakeCurrent(my_window->my_native_window, my_sdl_gl_context);
    log(LoggerName::ENGINE, "New SDL OpenGL context setup, version: {}.{}, AA samples: {}\n", my_major_version, my_minor_version, my_aa_samples);
}

void sdl_opengl_context::setup_gl_functions() const {
    ensures(gladLoadGLLoader(reinterpret_cast<GLADloadproc>(SDL_GL_GetProcAddress)) != 0, "Fail when loading GL functions from GLAD.");

    int major_version = 0, minor_version = 0;
    glGetIntegerv(GL_MAJOR_VERSION, &major_version);
    glGetIntegerv(GL_MINOR_VERSION, &minor_version);
    ensures(major_version == my_major_version && minor_version == my_minor_version, "Mismatch between GL version stored in application and the one return with OpenGL API.");
}

void sdl_opengl_context::swap_buffers() const {
    SDL_GL_SwapWindow(my_window->my_native_window);
}

void setup_default_gl_rendering_options() {
    glFrontFace(GL_CCW);
    glEnable(GL_MULTISAMPLE);
    // For freetype
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}