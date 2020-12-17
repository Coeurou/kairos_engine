#pragma once

#include <graphics/opengl/buffer_object.h>
#include <imgui_renderer.h>
#include <graphics/opengl/opengl_context.h>
#include <graphics/opengl/program.h>
#include <graphics/opengl/shader.h>

namespace kairos {

class imgui_opengl_renderer : public imgui_renderer {
  public:
    imgui_opengl_renderer(const opengl_context& gl_context);

    bool setup() override;
    void cleanup() override;
    void render(ImDrawData* draw_data) override;

    opengl_context my_gl_context;

  private:
    bool setup_opengl_data();
    void cleanup_opengl_data();

    bool setup_font_texture();
    void cleanup_font_texture();

    void setup_render_state(ImDrawData* draw_data, int fb_width, int fb_height,
                            GLuint vertex_array_object);

    // OpenGL Data
    GLuint my_font_texture = 0;

    program my_program{};
    shader my_vertex_shader{};
    shader my_fragment_shader{};

    GLint my_attribute_location_texture = 0;
    GLint my_attribute_location_projection_matrix = 0;

    GLuint my_attribute_location_vertex_pos = 0;
    GLuint my_attribute_location_vertex_uv = 0;
    GLuint my_attribute_location_vertex_color = 0;

    buffer_object my_vbo{};
    buffer_object my_ibo{};
};
} // namespace kairos

// Specific OpenGL ES versions
//#define IMGUI_IMPL_OPENGL_ES2     // Auto-detected on Emscripten
//#define IMGUI_IMPL_OPENGL_ES3     // Auto-detected on iOS/Android

// Attempt to auto-detect the default Desktop GL loader based on available header files.
// If auto-detection fails or doesn't select the same GL loader file as used by your application,
// you are likely to get a crash in ImGui_ImplOpenGL3_Init().
// You can explicitly select a loader by using one of the '#define IMGUI_IMPL_OPENGL_LOADER_XXX' in
// imconfig.h or compiler command-line.
#if !defined(IMGUI_IMPL_OPENGL_ES2) && !defined(IMGUI_IMPL_OPENGL_ES3) &&                          \
    !defined(IMGUI_IMPL_OPENGL_LOADER_GLAD)

// Try to detect GLES on matching platforms
#if defined(__APPLE__)
#include "TargetConditionals.h"
#endif
#if (defined(__APPLE__) && (TARGET_OS_IOS || TARGET_OS_TV)) || (defined(__ANDROID__))
#define IMGUI_IMPL_OPENGL_ES3 // iOS, Android  -> GL ES 3, "#version 300 es"
#elif defined(__EMSCRIPTEN__)
#define IMGUI_IMPL_OPENGL_ES2 // Emscripten    -> GL ES 2, "#version 100"

// Otherwise try to detect supported Desktop OpenGL loaders..
#elif defined(__has_include)
#elif __has_include(<glad/glad.h>)
#define IMGUI_IMPL_OPENGL_LOADER_GLAD
#elif __has_include(<GL/gl3w.h>)
#define IMGUI_IMPL_OPENGL_LOADER_GL3W
#else
#error "Cannot detect OpenGL loader!"
#endif

#endif
