//----------------------------------------
// OpenGL    GLSL      GLSL
// version   version   string
//----------------------------------------
//  2.0       110       "#version 110"
//  2.1       120       "#version 120"
//  3.0       130       "#version 130"
//  3.1       140       "#version 140"
//  3.2       150       "#version 150"
//  3.3       330       "#version 330 core"
//  4.0       400       "#version 400 core"
//  4.1       410       "#version 410 core"
//  4.2       420       "#version 410 core"
//  4.3       430       "#version 430 core"
//  ES 2.0    100       "#version 100"      = WebGL 1.0
//  ES 3.0    300       "#version 300 es"   = WebGL 2.0
//----------------------------------------

#if defined(_MSC_VER) && !defined(_CRT_SECURE_NO_WARNINGS)
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <imgui_opengl_renderer.h>

#include <stdint.h> // intptr_t
#include <imgui.h>

#include <core/contract.h>

// GL includes
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#elif defined(IMGUI_IMPL_OPENGL_ES3)
#if (defined(__APPLE__) && (TARGET_OS_IOS || TARGET_OS_TV))
#include <OpenGLES/ES3/gl.h> // Use GL ES 3
#else
#include <GLES3/gl3.h> // Use GL ES 3
#endif
#endif

// Desktop GL 3.2+ has glDrawElementsBaseVertex() which GL ES and WebGL don't have.
#if defined(IMGUI_IMPL_OPENGL_ES2) || defined(IMGUI_IMPL_OPENGL_ES3) || !defined(GL_VERSION_3_2)
#define IMGUI_IMPL_OPENGL_MAY_HAVE_VTX_OFFSET 0
#else
#define IMGUI_IMPL_OPENGL_MAY_HAVE_VTX_OFFSET 1
#endif

namespace kairos {

imgui_opengl_renderer::imgui_opengl_renderer(const opengl_context& gl_context)
    : my_gl_context(gl_context) {}

bool imgui_opengl_renderer::setup() {
    ImGuiIO& io = ImGui::GetIO();
    io.BackendRendererName = "imgui_impl_opengl";
#if IMGUI_IMPL_OPENGL_MAY_HAVE_VTX_OFFSET
    if (version(my_gl_context) >= 320)
        io.BackendFlags |=
            ImGuiBackendFlags_RendererHasVtxOffset; // We can honor the ImDrawCmd::VtxOffset field,
                                                    // allowing for large meshes.
#endif

    return setup_opengl_data();
}

bool imgui_opengl_renderer::setup_opengl_data() {
    // Backup GL state
    GLint last_texture = 0;
    GLint last_array_buffer = 0;
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);
    glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &last_array_buffer);
#ifndef IMGUI_IMPL_OPENGL_ES2
    GLint last_vertex_array;
    glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &last_vertex_array);
#endif

    string gl_version = fmt::format("#version {}", version(my_gl_context));

    const GLchar* vertex_shader_glsl_300_es = "precision mediump float;\n"
                                              "layout (location = 0) in vec2 Position;\n"
                                              "layout (location = 1) in vec2 UV;\n"
                                              "layout (location = 2) in vec4 Color;\n"
                                              "uniform mat4 ProjMtx;\n"
                                              "out vec2 Frag_UV;\n"
                                              "out vec4 Frag_Color;\n"
                                              "void main()\n"
                                              "{\n"
                                              "    Frag_UV = UV;\n"
                                              "    Frag_Color = Color;\n"
                                              "    gl_Position = ProjMtx * vec4(Position.xy,0,1);\n"
                                              "}\n";

    const GLchar* vertex_shader_glsl_410_core =
        "layout (location = 0) in vec2 Position;\n"
        "layout (location = 1) in vec2 UV;\n"
        "layout (location = 2) in vec4 Color;\n"
        "uniform mat4 ProjMtx;\n"
        "out vec2 Frag_UV;\n"
        "out vec4 Frag_Color;\n"
        "void main()\n"
        "{\n"
        "    Frag_UV = UV;\n"
        "    Frag_Color = Color;\n"
        "    gl_Position = ProjMtx * vec4(Position.xy,0,1);\n"
        "}\n";

    const GLchar* fragment_shader_glsl_300_es =
        "precision mediump float;\n"
        "uniform sampler2D Texture;\n"
        "in vec2 Frag_UV;\n"
        "in vec4 Frag_Color;\n"
        "layout (location = 0) out vec4 Out_Color;\n"
        "void main()\n"
        "{\n"
        "    Out_Color = Frag_Color * texture(Texture, Frag_UV.st);\n"
        "}\n";

    const GLchar* fragment_shader_glsl_410_core =
        "in vec2 Frag_UV;\n"
        "in vec4 Frag_Color;\n"
        "uniform sampler2D Texture;\n"
        "layout (location = 0) out vec4 Out_Color;\n"
        "void main()\n"
        "{\n"
        "    Out_Color = Frag_Color * texture(Texture, Frag_UV.st);\n"
        "}\n";

    // Select shaders matching our GLSL versions
    const GLchar* vertex_shader = nullptr;
    const GLchar* fragment_shader = nullptr;
    if (version(my_gl_context) >= 410) {
        vertex_shader = vertex_shader_glsl_410_core;
        fragment_shader = fragment_shader_glsl_410_core;
    } else {
        vertex_shader = vertex_shader_glsl_300_es;
        fragment_shader = fragment_shader_glsl_300_es;
    }

    // Create shaders
    my_vertex_shader.my_shader_type = GL_VERTEX_SHADER;
    my_vertex_shader.my_shader_name = "imgui_vs";
    my_vertex_shader.my_shader_source = fmt::format("{}\n{}", gl_version, vertex_shader);
    ensures(my_vertex_shader.setup(), "[imgui_opengl_renderer::setup_opengl_data()] Couldn't setup vertex shader");

    my_fragment_shader.my_shader_type = GL_FRAGMENT_SHADER;
    my_fragment_shader.my_shader_name = "imgui_fs";
    my_fragment_shader.my_shader_source = fmt::format("{}\n{}", gl_version, fragment_shader);
    ensures(my_fragment_shader.setup(), "[imgui_opengl_renderer::setup_opengl_data()] Couldn't setup frag shader");

    ensures(my_program.setup({my_vertex_shader, my_fragment_shader}), "[imgui_opengl_renderer::setup_opengl_data()] Couldn't setup shader program");

    my_attribute_location_texture = glGetUniformLocation(my_program, "Texture");
    my_attribute_location_projection_matrix = glGetUniformLocation(my_program, "ProjMtx");
    my_attribute_location_vertex_pos =
        static_cast<GLuint>(glGetAttribLocation(my_program, "Position"));
    my_attribute_location_vertex_uv = static_cast<GLuint>(glGetAttribLocation(my_program, "UV"));
    my_attribute_location_vertex_color =
        static_cast<GLuint>(glGetAttribLocation(my_program, "Color"));

    // Create buffers
    glGenBuffers(1, &my_vbo.my_gl_id);
    my_vbo.my_target = buffer_target::geometry;
    glGenBuffers(1, &my_ibo.my_gl_id);
    my_ibo.my_target = buffer_target::indices;

    setup_font_texture();

    // Restore modified GL state
    glBindTexture(GL_TEXTURE_2D, last_texture);
    glBindBuffer(GL_ARRAY_BUFFER, last_array_buffer);
#ifndef IMGUI_IMPL_OPENGL_ES2
    glBindVertexArray(last_vertex_array);
#endif

    return true;
}

bool imgui_opengl_renderer::setup_font_texture() {
    // Build texture atlas
    ImGuiIO& io = ImGui::GetIO();
    unsigned char* pixels = nullptr;
    int width = 0;
    int height = 0;
    io.Fonts->GetTexDataAsRGBA32(
        &pixels, &width,
        &height); // Load as RGBA 32-bit (75% of the memory is wasted, but default font is so small)
                  // because it is more likely to be compatible with user's existing shaders. If
                  // your ImTextureId represent a higher-level concept than just a GL texture id,
                  // consider calling GetTexDataAsAlpha8() instead to save on GPU memory.

    // Upload texture to graphics system
    GLint last_texture;
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);
    glGenTextures(1, &my_font_texture);
    glBindTexture(GL_TEXTURE_2D, my_font_texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
#ifdef GL_UNPACK_ROW_LENGTH
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
#endif
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

    // Store our identifier
    io.Fonts->TexID = (ImTextureID)(intptr_t)my_font_texture;

    // Restore state
    glBindTexture(GL_TEXTURE_2D, last_texture);

    return true;
}

void imgui_opengl_renderer::cleanup() { cleanup_opengl_data(); }

void imgui_opengl_renderer::cleanup_opengl_data() {
    my_vbo.cleanup();
    my_ibo.cleanup();
    my_program.cleanup();
    my_vertex_shader.cleanup();
    my_fragment_shader.cleanup();

    cleanup_font_texture();
}

void imgui_opengl_renderer::cleanup_font_texture() {
    ImGuiIO& io = ImGui::GetIO();
    glDeleteTextures(1, &my_font_texture);
    my_font_texture = 0;
    io.Fonts->TexID = 0;
}

void imgui_opengl_renderer::render(ImDrawData* draw_data) {
    // Avoid rendering when minimized, scale coordinates for retina displays (screen coordinates !=
    // framebuffer coordinates)
    int fb_width = static_cast<int>(draw_data->DisplaySize.x * draw_data->FramebufferScale.x);
    int fb_height = static_cast<int>(draw_data->DisplaySize.y * draw_data->FramebufferScale.y);
    if (fb_width <= 0 || fb_height <= 0) {
        return;
    }

    // Backup GL state
    GLenum last_active_texture;
    glGetIntegerv(GL_ACTIVE_TEXTURE, (GLint*)&last_active_texture);
    glActiveTexture(GL_TEXTURE0);
    GLuint last_program;
    glGetIntegerv(GL_CURRENT_PROGRAM, (GLint*)&last_program);
    GLuint last_texture;
    glGetIntegerv(GL_TEXTURE_BINDING_2D, (GLint*)&last_texture);
#ifdef GL_SAMPLER_BINDING
    GLuint last_sampler;
    glGetIntegerv(GL_SAMPLER_BINDING, (GLint*)&last_sampler);
#endif
    GLuint last_array_buffer;
    glGetIntegerv(GL_ARRAY_BUFFER_BINDING, (GLint*)&last_array_buffer);
#ifndef IMGUI_IMPL_OPENGL_ES2
    GLuint last_vertex_array_object;
    glGetIntegerv(GL_VERTEX_ARRAY_BINDING, (GLint*)&last_vertex_array_object);
#endif
#ifdef GL_POLYGON_MODE
    GLint last_polygon_mode[2];
    glGetIntegerv(GL_POLYGON_MODE, last_polygon_mode);
#endif
    GLint last_viewport[4];
    glGetIntegerv(GL_VIEWPORT, last_viewport);
    GLint last_scissor_box[4];
    glGetIntegerv(GL_SCISSOR_BOX, last_scissor_box);
    GLenum last_blend_src_rgb;
    glGetIntegerv(GL_BLEND_SRC_RGB, (GLint*)&last_blend_src_rgb);
    GLenum last_blend_dst_rgb;
    glGetIntegerv(GL_BLEND_DST_RGB, (GLint*)&last_blend_dst_rgb);
    GLenum last_blend_src_alpha;
    glGetIntegerv(GL_BLEND_SRC_ALPHA, (GLint*)&last_blend_src_alpha);
    GLenum last_blend_dst_alpha;
    glGetIntegerv(GL_BLEND_DST_ALPHA, (GLint*)&last_blend_dst_alpha);
    GLenum last_blend_equation_rgb;
    glGetIntegerv(GL_BLEND_EQUATION_RGB, (GLint*)&last_blend_equation_rgb);
    GLenum last_blend_equation_alpha;
    glGetIntegerv(GL_BLEND_EQUATION_ALPHA, (GLint*)&last_blend_equation_alpha);
    GLboolean last_enable_blend = glIsEnabled(GL_BLEND);
    GLboolean last_enable_cull_face = glIsEnabled(GL_CULL_FACE);
    GLboolean last_enable_depth_test = glIsEnabled(GL_DEPTH_TEST);
    GLboolean last_enable_scissor_test = glIsEnabled(GL_SCISSOR_TEST);

    // Setup desired GL state
    // Recreate the VAO every time (this is to easily allow multiple GL contexts to be rendered to.
    // VAO are not shared among GL contexts) The renderer would actually work without any VAO bound,
    // but then our VertexAttrib calls would overwrite the default one currently bound.
    GLuint vertex_array_object = 0;
#ifndef IMGUI_IMPL_OPENGL_ES2
    glGenVertexArrays(1, &vertex_array_object);
#endif
    setup_render_state(draw_data, fb_width, fb_height, vertex_array_object);

    // Will project scissor/clipping rectangles into framebuffer space
    ImVec2 clip_off = draw_data->DisplayPos; // (0,0) unless using multi-viewports
    ImVec2 clip_scale =
        draw_data->FramebufferScale; // (1,1) unless using retina display which are often (2,2)

    // Render command lists
    for (int n = 0; n < draw_data->CmdListsCount; n++) {
        const ImDrawList* cmd_list = draw_data->CmdLists[n];

        // Upload vertex/index buffers
        glBufferData(GL_ARRAY_BUFFER,
                     (GLsizeiptr)cmd_list->VtxBuffer.Size * (int)sizeof(ImDrawVert),
                     (const GLvoid*)cmd_list->VtxBuffer.Data, GL_STREAM_DRAW);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                     (GLsizeiptr)cmd_list->IdxBuffer.Size * (int)sizeof(ImDrawIdx),
                     (const GLvoid*)cmd_list->IdxBuffer.Data, GL_STREAM_DRAW);

        for (int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.Size; cmd_i++) {
            const ImDrawCmd* pcmd = &cmd_list->CmdBuffer[cmd_i];
            if (pcmd->UserCallback != NULL) {
                // User callback, registered via ImDrawList::AddCallback()
                // (ImDrawCallback_ResetRenderState is a special callback value used by the user to
                // request the renderer to reset render state.)
                if (pcmd->UserCallback == ImDrawCallback_ResetRenderState)
                    setup_render_state(draw_data, fb_width, fb_height, vertex_array_object);
                else
                    pcmd->UserCallback(cmd_list, pcmd);
            } else {
                // Project scissor/clipping rectangles into framebuffer space
                ImVec4 clip_rect;
                clip_rect.x = (pcmd->ClipRect.x - clip_off.x) * clip_scale.x;
                clip_rect.y = (pcmd->ClipRect.y - clip_off.y) * clip_scale.y;
                clip_rect.z = (pcmd->ClipRect.z - clip_off.x) * clip_scale.x;
                clip_rect.w = (pcmd->ClipRect.w - clip_off.y) * clip_scale.y;

                if (clip_rect.x < fb_width && clip_rect.y < fb_height && clip_rect.z >= 0.0f &&
                    clip_rect.w >= 0.0f) {
                    // Apply scissor/clipping rectangle
                    glScissor((int)clip_rect.x, (int)(fb_height - clip_rect.w),
                              (int)(clip_rect.z - clip_rect.x), (int)(clip_rect.w - clip_rect.y));

                    // Bind texture, Draw
                    glBindTexture(GL_TEXTURE_2D, (GLuint)(intptr_t)pcmd->TextureId);
#if IMGUI_IMPL_OPENGL_MAY_HAVE_VTX_OFFSET
                    if (version(my_gl_context) >= 320)
                        glDrawElementsBaseVertex(
                            GL_TRIANGLES, (GLsizei)pcmd->ElemCount,
                            sizeof(ImDrawIdx) == 2 ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT,
                            (void*)(intptr_t)(pcmd->IdxOffset * sizeof(ImDrawIdx)),
                            (GLint)pcmd->VtxOffset);
                    else
#endif
                        glDrawElements(GL_TRIANGLES, (GLsizei)pcmd->ElemCount,
                                       sizeof(ImDrawIdx) == 2 ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT,
                                       (void*)(intptr_t)(pcmd->IdxOffset * sizeof(ImDrawIdx)));
                }
            }
        }
    }

    // Destroy the temporary VAO
#ifndef IMGUI_IMPL_OPENGL_ES2
    glDeleteVertexArrays(1, &vertex_array_object);
#endif

    // Restore modified GL state
    glUseProgram(last_program);
    glBindTexture(GL_TEXTURE_2D, last_texture);
#ifdef GL_SAMPLER_BINDING
    glBindSampler(0, last_sampler);
#endif
    glActiveTexture(last_active_texture);
#ifndef IMGUI_IMPL_OPENGL_ES2
    glBindVertexArray(last_vertex_array_object);
#endif
    glBindBuffer(GL_ARRAY_BUFFER, last_array_buffer);
    glBlendEquationSeparate(last_blend_equation_rgb, last_blend_equation_alpha);
    glBlendFuncSeparate(last_blend_src_rgb, last_blend_dst_rgb, last_blend_src_alpha,
                        last_blend_dst_alpha);
    if (last_enable_blend)
        glEnable(GL_BLEND);
    else
        glDisable(GL_BLEND);
    if (last_enable_cull_face)
        glEnable(GL_CULL_FACE);
    else
        glDisable(GL_CULL_FACE);
    if (last_enable_depth_test)
        glEnable(GL_DEPTH_TEST);
    else
        glDisable(GL_DEPTH_TEST);
    if (last_enable_scissor_test)
        glEnable(GL_SCISSOR_TEST);
    else
        glDisable(GL_SCISSOR_TEST);
#ifdef GL_POLYGON_MODE
    glPolygonMode(GL_FRONT_AND_BACK, (GLenum)last_polygon_mode[0]);
#endif
    glViewport(last_viewport[0], last_viewport[1], (GLsizei)last_viewport[2],
               (GLsizei)last_viewport[3]);
    glScissor(last_scissor_box[0], last_scissor_box[1], (GLsizei)last_scissor_box[2],
              (GLsizei)last_scissor_box[3]);
}

void imgui_opengl_renderer::setup_render_state(ImDrawData* draw_data, int fb_width, int fb_height,
                                               GLuint vertex_array_object) {
    // Setup render state: alpha-blending enabled, no face culling, no depth testing, scissor
    // enabled, polygon fill
    glEnable(GL_BLEND);
    glBlendEquation(GL_FUNC_ADD);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_SCISSOR_TEST);
#ifdef GL_POLYGON_MODE
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
#endif

    // Support for GL 4.5 rarely used glClipControl(GL_UPPER_LEFT)
    bool clip_origin_lower_left = true;
#if defined(GL_CLIP_ORIGIN) && !defined(__APPLE__)
    GLenum current_clip_origin = 0;
    glGetIntegerv(GL_CLIP_ORIGIN, (GLint*)&current_clip_origin);
    if (current_clip_origin == GL_UPPER_LEFT)
        clip_origin_lower_left = false;
#endif

    // Setup viewport, orthographic projection matrix
    // Our visible imgui space lies from draw_data->DisplayPos (top left) to
    // draw_data->DisplayPos+data_data->DisplaySize (bottom right). DisplayPos is (0,0) for single
    // viewport apps.
    glViewport(0, 0, (GLsizei)fb_width, (GLsizei)fb_height);
    float L = draw_data->DisplayPos.x;
    float R = draw_data->DisplayPos.x + draw_data->DisplaySize.x;
    float T = draw_data->DisplayPos.y;
    float B = draw_data->DisplayPos.y + draw_data->DisplaySize.y;
    if (!clip_origin_lower_left) {
        float tmp = T;
        T = B;
        B = tmp;
    } // Swap top and bottom if origin is upper left
    const float ortho_projection[4][4] = {
        {2.0f / (R - L), 0.0f, 0.0f, 0.0f},
        {0.0f, 2.0f / (T - B), 0.0f, 0.0f},
        {0.0f, 0.0f, -1.0f, 0.0f},
        {(R + L) / (L - R), (T + B) / (B - T), 0.0f, 1.0f},
    };
    glUseProgram(my_program);
    glUniform1i(my_attribute_location_texture, 0);
    glUniformMatrix4fv(my_attribute_location_projection_matrix, 1, GL_FALSE,
                       &ortho_projection[0][0]);
#ifdef GL_SAMPLER_BINDING
    glBindSampler(0, 0); // We use combined texture/sampler state. Applications using GL 3.3 may set
                         // that otherwise.
#endif

    (void)vertex_array_object;
#ifndef IMGUI_IMPL_OPENGL_ES2
    glBindVertexArray(vertex_array_object);
#endif

    // Bind vertex/index buffers and setup attributes for ImDrawVert
    my_vbo.bind();
    my_ibo.bind();
    glEnableVertexAttribArray(my_attribute_location_vertex_pos);
    glEnableVertexAttribArray(my_attribute_location_vertex_uv);
    glEnableVertexAttribArray(my_attribute_location_vertex_color);
    glVertexAttribPointer(my_attribute_location_vertex_pos, 2, GL_FLOAT, GL_FALSE,
                          sizeof(ImDrawVert), (GLvoid*)IM_OFFSETOF(ImDrawVert, pos));
    glVertexAttribPointer(my_attribute_location_vertex_uv, 2, GL_FLOAT, GL_FALSE,
                          sizeof(ImDrawVert), (GLvoid*)IM_OFFSETOF(ImDrawVert, uv));
    glVertexAttribPointer(my_attribute_location_vertex_color, 4, GL_UNSIGNED_BYTE, GL_TRUE,
                          sizeof(ImDrawVert), (GLvoid*)IM_OFFSETOF(ImDrawVert, col));
}

} // namespace kairos