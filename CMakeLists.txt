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

add_sources(KAIROS_SOURCES engine/graphics/font engine/graphics/program engine/graphics/shader engine/graphics/ibo engine/graphics/vao engine/graphics/vbo engine/graphics/texture engine/graphics/brush engine/graphics/painter engine/graphics/canvas engine/graphics/rendering_layer engine/graphics/opengl_helper engine/core/window engine/graphics/opengl_context engine/core/command_line_parser engine/core/application engine/core/event_listener
                           engine/core/logger engine/core/mathlib engine/core/messages
                           )

add_headers(KAIROS_HEADERS engine/graphics/font engine/graphics/program engine/graphics/shader engine/graphics/ibo engine/graphics/vao engine/graphics/vbo engine/graphics/texture engine/graphics/brush engine/core/rect engine/graphics/pen engine/graphics/painter engine/graphics/canvas engine/graphics/rendering_layer engine/graphics/opengl_helper engine/core/window engine/graphics/opengl_context engine/core/command_line_parser engine/core/filesystem engine/core/globals engine/core/application engine/core/event_listener
                           engine/core/error
                           engine/core/json engine/core/logger
                           engine/core/mathlib engine/core/messages engine/graphics/stb_image engine/core/types)

add_library(kairos_engine ${KAIROS_HEADERS} ${KAIROS_SOURCES})

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
        ${PROJECT_SOURCE_DIR}/source/engine/core
        ${PROJECT_SOURCE_DIR}/source/engine/graphics
        extern/cxxopts/include
        extern/fmt/include
        extern/nameof/include
        extern/glad/include
        extern/glm)

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
    target_precompile_headers(project_options PRIVATE types.h)
endif ()

# glfw
add_subdirectory(extern/glfw EXCLUDE_FROM_ALL)

# glad
add_subdirectory(extern/glad EXCLUDE_FROM_ALL)

# glm
add_subdirectory(extern/glm EXCLUDE_FROM_ALL)

# freetype
add_subdirectory(extern/freetype-2.10.0 EXCLUDE_FROM_ALL)

# fmt
add_subdirectory(extern/fmt EXCLUDE_FROM_ALL)

# json
add_subdirectory(extern/json EXCLUDE_FROM_ALL)

# libsndfile
add_subdirectory(extern/libsndfile EXCLUDE_FROM_ALL)

# openal-soft
add_subdirectory(extern/openal-soft EXCLUDE_FROM_ALL)

# tests
if (ENABLE_TESTING)
    enable_testing()
    message("Building Tests")
    add_subdirectory(tests)
endif ()

add_subdirectory(source)

# Link dependencies
target_link_libraries(kairos_engine fmt glad glfw freetype OpenAL)
