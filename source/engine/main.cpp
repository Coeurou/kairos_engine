#if defined(WIN32)/* && defined(NDEBUG)*/

#include <windows.h>

extern int main(int argc, char* argv[]);

////////////////////////////////////////////////////////////
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, INT) { return main(__argc, __argv); }

#endif // WIN32

#include <SDL.h>
#undef main

#include <command_line_parser.h>
#include <editor.h>
#include <formattable.h>
#include <globals.h>
#include <logger.h>
#include <sdl_application.h>
#include <sdl_window.h>

int main(int argc, char* argv[]) {
    // init app data
    editor engine_editor{};
    {
        our_application = &our_sdl_application;
        our_application->my_name = "GameEngine";
        our_application->my_version = "0.0.1";
        our_application->my_organisation = "Kairos";
        our_application->my_execution_callbacks.emplace_back([&engine_editor]() { 
            engine_editor.update();
            engine_editor.render();
        });
        our_application->my_events_callbacks.emplace_back([&engine_editor](const variant_event& e) {
            engine_editor.poll_event(e);
        });
    }
    core_application::init();

    command_line_parser options(argv[0], " - game");
    options.add_options()("resolution", "Window resolution",
        cxxopts::value<array<int>>()->default_value("1920,1080"))
        ("gl", "The OpenGL version for creating context", cxxopts::value<array<unsigned char>>()->default_value("4,1"))
        ("help", "Show help");

    window_params window_params{};
    window_params.my_title = our_application->my_name;

    try {
        parse_result arguments = options.parse(argc, argv);

        // create window
        {
            const auto window_size = arguments["resolution"].as<array<int>>();
            window_params.my_size.x = static_cast<float>(window_size[0]);
            window_params.my_size.y = static_cast<float>(window_size[1]);

            our_application->my_main_window = create_window<sdl_window>(window_params);
        }

        // create OpenGL context
        our_application->my_gl_context = std::make_unique<sdl_opengl_context>(*core_application::window());
        auto gl_context = our_application->my_gl_context.get();
        {
            const auto gl_version = arguments["gl"].as<array<uint8>>();
            const auto antialiasing_samples = 4;
            gl_context->my_major_version = gl_version[0];
            gl_context->my_minor_version = gl_version[1];
            gl_context->my_aa_samples = antialiasing_samples;
            gl_context->my_profile = opengl_context::opengl_profile::core;
            gl_context->setup_gl_context();
            gl_context->setup_gl_functions();
            setup_default_gl_rendering_options();
        }
        if (gl_context->get_version() != 0) {
            core_application::set_graphics_implementation((gl_context->my_profile == opengl_context::opengl_profile::es) ? renderer_engine::opengl_es : renderer_engine::opengl);
        }
    }
    catch (const cxxopts::OptionException& e) {
        log_error(LoggerName::ENGINE, "error parsing command line options: {}\n", e.what());
        return EXIT_FAILURE;
    }

    vec2i framebuffer = core_application::window()->size();
    globals::pixel_size = { static_cast<float>(framebuffer.x) / window_params.my_size.x,
                           static_cast<float>(framebuffer.y) / window_params.my_size.y };
    log(LoggerName::ENGINE, "Pixel size set to {}\n", to_string(globals::pixel_size));


    engine_editor.setup();
    core_application::exec();

    engine_editor.cleanup();
    core_application::exit();

    return EXIT_SUCCESS;
}