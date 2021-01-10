#if defined(WIN32) /* && defined(NDEBUG)*/

#include <windows.h>

extern int main(int argc, char* argv[]);

////////////////////////////////////////////////////////////
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, INT) { return main(__argc, __argv); }

#endif // WIN32

#include <SDL.h>
#undef main

#include <editor.h>
#include <game.h>
#include <core/animation.h>
#include <core/contract.h>
#include <core/event.h>
#include <core/security.h>
#include <core/time.h>
#include <event/sdl_event.h>
#include <graphics/opengl/opengl_renderer.h>
#include <graphics/opengl/SDL/sdl_opengl_context.h>
#include <graphics/service/video_service.h>
#include <graphics/texture.h>
#include <graphics/brush.h>
#include <graphics/pen.h>
#include <graphics/painter.h>
#include <imgui_sdl_backend.h>
#include <imgui_opengl_renderer.h>
#include <input/service/input_service.h>
#include <window/sdl_window.h>

using namespace kairos;

int main(int /*argc*/, char* argv[]) {
    if (!is_unique_instance(argv[0])) {
        log_error(LoggerName::SYSTEM, "Another instance is already running");
        return EXIT_FAILURE;
    }

    video_service video;
    video.enable();

    input_service input;
    input.enable(input_device::controller);

    // create opengl context
    opengl_context gl_context{sdl_opengl_context{4, 1}};
    {
        set_attribute(gl_context, gl_attribute::double_buffering, 1);
        set_attribute(gl_context, gl_attribute::multisample_buffers, 1);
        set_attribute(gl_context, gl_attribute::aa_samples, 4);
        opengl_context::opengl_profile profile = opengl_context::opengl_profile::core;
        set_attribute(gl_context, gl_attribute::context_profile, static_cast<int>(profile));
    }

    // create window
    window_params window_params{
        "Kairos",
        {1600.f, 900.f},
        pointf{window_params::our_undefined_pos, window_params::our_undefined_pos},
        SDL_WINDOW_OPENGL | SDL_WINDOW_HIDDEN};
    window w{sdl_window{window_params}};
    {
        ensures(setup(gl_context,
                      id(w)), "Couldn't setup opengl context"); // need to comment this function setup is okay since in opengl it
                               // has a meaning but adding an explanation is necessary
        show(w);
    }

    // imgui
    editor imgui = editor(imgui_sdl_backend{}, imgui_opengl_renderer(gl_context));
    imgui.setup(w);

    // rendering parameters
    auto agent_texture = load("../../assets/Alucard.png");
    ensures(agent_texture.has_value());

    vec2i tex_size = size(*agent_texture);
    rect<int> texture_rect = {{10, 15}, {33, 62}};
    sprite agent{*agent_texture, rectf{{0.f, 450.f}, {64.f, 546.f}}, texture_rect};
    flip(agent, axis::y, true);

    brush b;
    b.my_fill_color = color(1.f);

    pen p;
    p.my_line_width = 1.f;

    painter painter{opengl_renderer{p, b, size(w)}};

    // logic parameters
    auto time = clock::get_time();
    bool should_quit = false;

    // physics parameters
    constexpr float speed = 4.f;

    // animation parameters
    animation<rect<int>> idle_animation = animation<rect<int>>({{{62, 14}, {86, 62}},
                                                                {{97, 14}, {121, 62}},
                                                                {{131, 14}, {155, 62}},
                                                                {{166, 14}, {190, 62}},
                                                                {{199, 14}, {223, 62}},
                                                                {{233, 14}, {257, 62}}}, 1.f, 1.f, true);
    idle_animation.start();

    system_event_queue event_queue{sdl_event_tag{}};
    array<system_event> events;
    // this should be a function for input, like intensity(keyboard_event)
    const auto to_float = [](bool condition) { return (condition) ? 1.f : 0.f; };

    while (!should_quit) {
        // frame variables
        pointf agent_movement = {0.f, 0.f};

        // peek frame events
        peek_events(event_queue, events);
        for (const auto& e : events) {
            switch (type(e)) {
            case event_type::window: {
                should_quit |= (cast<window_event>(e).my_type == window_event_type::close);
                break;
            }
            case event_type::key_press: {
                const auto& key_event = cast<keyboard_event>(e);
                if (key_event.my_key == key::D) {
                    agent_movement.x = 1.f * speed;
                } else if (key_event.my_key == key::A) {
                    agent_movement.x = -1.f * speed;
                }
                break;
            }
            case event_type::key_release: {
                break;
            }
            }
            should_quit |= (type(e) == event_type::quit);
            imgui.process_event(e);
        }

        // update logic
        const auto current_time = clock::get_time();
        float deltatime = static_cast<float>(current_time - time) * 0.001f;
        time = current_time;

        const auto anim_sample = idle_animation.update(deltatime);
        if (anim_sample) {
            agent.my_texture_rect = *anim_sample;
        }

        // render
        clear(painter, color(1.f, 0.57f, 0.2f, 1.f));
        translate(agent.my_bounds, agent_movement);
        draw(painter, agent);
        ensures(bool(imgui.update()), "imgui update failure");
        ensures(bool(imgui.render()), "imgui render failure");

        swap_buffers(gl_context);
    }

    // cleanup
    destroy(gl_context);
    destroy(w);

    video.cleanup();

    return EXIT_SUCCESS;
}