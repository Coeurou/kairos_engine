#pragma once

#include <opengl_context.h>

#include <application.h>
#include <error.h>

void opengl_context::setup_gl_context() const {
    if (!glfwInit()) {
        const char* description = nullptr;
        int code = glfwGetError(&description);
        const string error_msg = fmt::format("GLFW failure: {}, reason: {}", code, description);
        ASSERT_MSG(false, error_msg.c_str());
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major_version);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor_version);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, (profile == opengl_profile::core) ? GLFW_OPENGL_CORE_PROFILE : GLFW_OPENGL_COMPAT_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, aa_samples);
    //glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
}

void opengl_context::setup_gl_functions() const {
    bool is_gl_functions_loaded = (gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)) != 0);
    ASSERT_MSG(is_gl_functions_loaded, "Fail when loading GL functions from GLAD.");
}

void opengl_context::swap_buffers() const {
    glfwSwapBuffers(application::native_window());
}

void setup_default_gl_rendering_options() {
    glfwSwapInterval(1);

    glFrontFace(GL_CCW);
    glEnable(GL_MULTISAMPLE);
    // For freetype
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}