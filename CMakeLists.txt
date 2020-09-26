cmake_minimum_required(VERSION 3.15)

project(kairos_engine VERSION 0.1.0)
include(cmake/StandardProjectSettings.cmake)

# Link this 'library' to set the c++ standard / compile-time options requested
add_library(project_options INTERFACE)
target_compile_features(project_options INTERFACE cxx_std_17)
set(CMAKE_C_STANDARD 11)

function(add_sources VAR)
  set(all_files ${${VAR}})
  foreach (file ${ARGN})
    set(all_files ${all_files} source/${file}.cpp)
  endforeach()
  set(${VAR} ${all_files} PARENT_SCOPE)
endfunction()

function(add_headers VAR)
  set(all_files ${${VAR}})
  foreach (file ${ARGN})
    set(all_files ${all_files} source/${file}.h)
  endforeach()
  set(${VAR} ${all_files} PARENT_SCOPE)
endfunction()

add_sources(KAIROS_SOURCES engine/graphics/opengl/opengl_draw_data engine/core/opengl_renderer editor/imgui_console_command editor/error_command editor/command_parser 
                           editor/log_command editor/editor editor/imgui_opengl_renderer editor/imgui_sdl_backend
                           engine/window/sdl_window engine/application/sdl_application engine/application/log_application
                           engine/application/core_application  engine/application/command_line_parser
                           engine/math/transform engine/graphics/log_renderer engine/systems/shader_system
                           engine/graphics/font engine/graphics/opengl/program engine/graphics/opengl/shader
                           engine/graphics/opengl/vao engine/graphics/opengl/buffer_object engine/graphics/texture
                           engine/graphics/brush engine/graphics/painter engine/graphics/canvas
                           engine/graphics/rendering_layer
                           engine/graphics/opengl/opengl_context
                           engine/core/event_listener engine/core/logger engine/math/mathlib engine/core/messages
                           )

add_headers(KAIROS_HEADERS engine/graphics/material engine/graphics/opengl/opengl_draw_data engine/core/opengl_renderer editor/imgui_console_command editor/error_command editor/command_parser
                           editor/log_command editor/command_interface editor/command_history 
                           editor/editor editor/imgui_backend editor/imgui_renderer editor/imgui_opengl_renderer
                           editor/imgui_sdl_backend engine/core/contract 
                           engine/window/window engine/window/sdl_window 
                           engine/application/application engine/application/sdl_application
                           engine/application/log_application engine/application/core_application engine/application/command_line_parser 
                           engine/serialization/serialization_strategy engine/serialization/serializer engine/math/transform 
                           engine/core/formattable engine/core/makable engine/graphics/log_renderer 
                           engine/graphics/2d_renderer_interface engine/systems/shader_system engine/core/kairos_lib 
                           engine/graphics/vertex engine/graphics/font engine/graphics/opengl/program engine/graphics/opengl/shader 
                           engine/graphics/opengl/vao engine/graphics/opengl/buffer_object engine/graphics/texture 
                           engine/graphics/brush engine/math/rect engine/graphics/pen engine/graphics/painter 
                           engine/graphics/canvas engine/graphics/rendering_layer
                           engine/graphics/opengl/opengl_context
                           engine/core/filesystem engine/core/globals
                           engine/core/event_listener engine/core/error
                           engine/core/json engine/core/logger
                           engine/math/mathlib engine/core/messages engine/graphics/stb_image engine/core/types)
set(IMGUI_SRCS extern/imgui/imgui.h extern/imgui/imgui.cpp extern/imgui/imconfig.h extern/imgui/imgui_internal.h extern/imgui/imgui_draw.cpp extern/imgui/imgui_demo.cpp extern/imgui/imgui_widgets.cpp extern/imgui/imstb_rectpack.h extern/imgui/imstb_textedit.h extern/imgui/imstb_truetype.h)
add_library(kairos_engine ${KAIROS_HEADERS} ${KAIROS_SOURCES} ${IMGUI_SRCS})

if (MSVC)
    function(group_sources VAR)
        foreach(src IN LISTS ${VAR})
            get_filename_component(source_path "${src}" PATH)
            string(REPLACE "/" "\\" source_path_msvc "${source_path}")
            source_group("${source_path_msvc}" FILES "${src}")
        endforeach()
    endfunction()

    group_sources(KAIROS_SOURCES)
    group_sources(KAIROS_HEADERS)
endif ()

# Add include dir
target_include_directories(kairos_engine PUBLIC
        ${PROJECT_SOURCE_DIR}/source/engine/application
        ${PROJECT_SOURCE_DIR}/source/engine/core
        ${PROJECT_SOURCE_DIR}/source/engine/graphics
        ${PROJECT_SOURCE_DIR}/source/engine/graphics/opengl
        ${PROJECT_SOURCE_DIR}/source/engine/math
        ${PROJECT_SOURCE_DIR}/source/engine/serialization
        ${PROJECT_SOURCE_DIR}/source/engine/systems
        ${PROJECT_SOURCE_DIR}/source/engine/window
        ${PROJECT_SOURCE_DIR}/source/editor
        extern/cxxopts/include
        extern/fmt/include
        extern/nameof/include
        extern/glad/include
        extern/glm
        extern/imgui
        extern/SDL2-2.0.12/include
        extern/SDL2_image-2.0.5/include
        extern/SDL2_mixer-2.0.4/include
        extern/SDL2_ttf-2.0.15/include)

# Link this 'library' to use the warnings specified in CompilerWarnings.cmake
add_library(project_warnings INTERFACE)

# standard compiler warnings
include(cmake/CompilerWarnings.cmake)
set_project_warnings(project_warnings)

# sanitizer options if supported by compiler
include(cmake/Sanitizers.cmake)
enable_sanitizers(project_options)

# enable doxygen
include(cmake/Doxygen.cmake)
enable_doxygen()

# allow for static analysis options
include(cmake/StaticAnalyzers.cmake)

option(BUILD_SHARED_LIBS "Enable compilation of shared libraries" OFF)
option(ENABLE_TESTING "Enable Test Builds" ON)

# Very basic PCH example
option(ENABLE_PCH "Enable Precompiled Headers" ON)
if (ENABLE_PCH)
    # This sets a global PCH parameter, each project will build its own PCH, which
    # is a good idea if any #define's change
    target_precompile_headers(project_options INTERFACE <vector> <string> <map> <utility>)
    target_precompile_headers(project_options PRIVATE types.h kairos_lib.h)
endif ()

# glad
add_subdirectory(extern/glad EXCLUDE_FROM_ALL)

# glm
add_subdirectory(extern/glm EXCLUDE_FROM_ALL)

# fmt
add_subdirectory(extern/fmt EXCLUDE_FROM_ALL)

# json
add_subdirectory(extern/json EXCLUDE_FROM_ALL)

# sdl2
add_subdirectory(extern/SDL2-2.0.12 EXCLUDE_FROM_ALL)

# tests
if (ENABLE_TESTING)
    enable_testing()
    message("Building Tests")
    add_subdirectory(tests)
endif ()

add_subdirectory(source)

# Link dependencies
target_link_libraries(kairos_engine fmt glad SDL2-static)
