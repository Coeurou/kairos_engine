#if defined(WIN32)/* && defined(NDEBUG)*/

#include <windows.h>

extern int main(int argc, char* argv[]);

////////////////////////////////////////////////////////////
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, INT) { return main(__argc, __argv); }

#endif // WIN32

#include <SDL.h>
#undef main

#include <editor.h>
#include <game.h>
#include <core/event.h>
#include <core/security.h>
#include <event/sdl_event.h>
#include <graphics/service/video_service.h>
#include <window/sdl_window.h>
#include <window/window.h>

using namespace kairos;

int main(int /*argc*/, char* argv[]) {
    if (!is_unique_instance(argv[0])) {
        log_error(LoggerName::SYSTEM, "Another instance is already running");
        return EXIT_FAILURE;
    }

    window_params window_params{};
    window_params.my_title = "Kairos";
    window_params.my_size = {1600.f, 900.f};
    window_params.my_flags = SDL_WINDOW_OPENGL | SDL_WINDOW_HIDDEN;

    video_service video;
    video.setup();

    window w{ sdl_window{window_params} };
    show(w);

    bool should_quit = false;
    system_event_queue event_queue{ sdl_event_tag{} };
    array<system_event> events;
    while (!should_quit) {
        peek_events(event_queue, events);
        for (const auto& e : events) {
            switch (e.my_type) {
            case event_type::window: {
                should_quit |= (std::get<window_event>(e.my_internal).my_type == window_event_type::close);
            }
            }
            should_quit |= (e.my_type == event_type::quit);
        }
    }

    destroy(w);
    return EXIT_SUCCESS;
}