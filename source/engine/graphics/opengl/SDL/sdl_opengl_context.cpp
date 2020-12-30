#include <graphics/opengl/SDL/sdl_opengl_context.h>

#include <core/algorithms.h>
#include <core/contract.h>

namespace kairos {

/** Member functions */

sdl_opengl_context::sdl_opengl_context(int major, int minor) {
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, major);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, minor);
}

uint32 sdl_opengl_context::get_window_id() const { return my_window_id; }
void sdl_opengl_context::set_window_id(uint32 window_id) { my_window_id = window_id; }

SDL_GLContext sdl_opengl_context::get_sdl_gl_context() const { return my_sdl_gl_context; }
void sdl_opengl_context::set_sdl_gl_context(SDL_GLContext context) { my_sdl_gl_context = context; }

/** Non-member functions */

bool setup(sdl_opengl_context& gl_context, uint32 window_id) {
    gl_context.set_window_id(window_id);
    SDL_Window* window = SDL_GetWindowFromID(gl_context.get_window_id());

    gl_context.set_sdl_gl_context(SDL_GL_CreateContext(window));

    SDL_GL_SetSwapInterval(1); // Enable vsync
    SDL_GL_MakeCurrent(window, gl_context.get_sdl_gl_context());

    log(LoggerName::ENGINE, "New SDL OpenGL context setup, version: {}.\n", version(gl_context));

    ensures(gladLoadGLLoader(reinterpret_cast<GLADloadproc>(SDL_GL_GetProcAddress)) != 0,
            "Fail when loading GL functions from GLAD.");

    glEnable(GL_MULTISAMPLE);
    glEnable(GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    int major_version = 0, minor_version = 0;
    glGetIntegerv(GL_MAJOR_VERSION, &major_version);
    glGetIntegerv(GL_MINOR_VERSION, &minor_version);
    int sdl_major_version = 0, sdl_minor_version = 0;
    SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &sdl_major_version);
    SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &sdl_minor_version);
    return (major_version == sdl_major_version && minor_version == sdl_minor_version);
}

int version(const sdl_opengl_context& /*gl_context*/) {
    int sdl_major_version = 0, sdl_minor_version = 0;
    SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &sdl_major_version);
    SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &sdl_minor_version);
    return sdl_major_version * 100 + sdl_minor_version * 10;
}

void swap_buffers(sdl_opengl_context& gl_context) {
    SDL_GL_SwapWindow(SDL_GetWindowFromID(gl_context.get_window_id()));
}

int get_attribute(const sdl_opengl_context& /*gl_context*/, gl_attribute attr) {
    const auto it = find_if(our_sdl_gl_attribute_adapter, [&attr](const auto& sdl_kairos_pair) {
        return sdl_kairos_pair.second == attr;
    });
    ensures(it != our_sdl_gl_attribute_adapter.end());

    int output = 0;
    SDL_GL_GetAttribute(it->first, &output);
    return output;
}

void set_attribute(sdl_opengl_context& /*gl_context*/, gl_attribute attr, int value) {
    const auto it = find_if(our_sdl_gl_attribute_adapter, [&attr](const auto& sdl_kairos_pair) {
        return sdl_kairos_pair.second == attr;
    });
    ensures(it != our_sdl_gl_attribute_adapter.end());

    if (attr == gl_attribute::context_profile) {
        value = our_sdl_gl_profile_adapter.at(static_cast<opengl_context::opengl_profile>(value));
    }
    SDL_GL_SetAttribute(it->first, value);
}

void destroy(sdl_opengl_context& gl_context) { SDL_GL_DeleteContext(gl_context.get_sdl_gl_context()); }

} // namespace kairos