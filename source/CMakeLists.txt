add_executable(game engine/main.cpp)

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    if (MSVC)
        set_target_properties(Engine PROPERTIES LINK_FLAGS /SUBSYSTEM:WINDOWS)
        set_property(TARGET Engine PROPERTY WIN32_EXECUTABLE true)
    else()
        set_property(TARGET Engine PROPERTY MACOSX_BUNDLE true)
    endif()
endif()

target_link_libraries(game PRIVATE project_options project_warnings kairos_engine)
