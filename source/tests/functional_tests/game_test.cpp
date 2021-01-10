#include <functional_tests/game_test.h>

#include <SDL.h>

#include <core/contract.h>
#include <core/time.h>

#include <event/sdl_event.h>
#include <graphics/opengl/opengl_renderer.h>
#include <graphics/opengl/SDL/sdl_opengl_context.h>
#include <graphics/brush.h>
#include <graphics/pen.h>
#include <window/sdl_window.h>

namespace kairos {

game_test::game_test() : my_event_queue{sdl_event_tag{}} {
    my_video.enable();
    my_window = window{sdl_window{
        window_params{"func_test",
                      {800.f, 600.f},
                      pointf{window_params::our_undefined_pos, window_params::our_undefined_pos},
                      SDL_WINDOW_OPENGL | SDL_WINDOW_HIDDEN}}};
    my_gl_context = opengl_context{sdl_opengl_context{4, 1}};
}

void game_test::add_logic_command(update_func cmd) {
    my_logic_commands.emplace_back(std::move(cmd));
}

void game_test::add_draw_command(draw_func cmd) { my_draw_commands.emplace_back(std::move(cmd)); }

void game_test::set_state(game_state state) { my_state = state; }

painter game_test::get_painter() const { return my_painter; }

window game_test::get_window() const { return my_window; }

void game_test::run() {
    switch (my_state) {
    case game_state::not_initialized: {
        set_attribute(my_gl_context, gl_attribute::double_buffering, 1);
        set_attribute(my_gl_context, gl_attribute::multisample_buffers, 1);
        set_attribute(my_gl_context, gl_attribute::aa_samples, 4);
        opengl_context::opengl_profile profile = opengl_context::opengl_profile::core;
        set_attribute(my_gl_context, gl_attribute::context_profile, static_cast<int>(profile));

        ensures(setup(my_gl_context, id(my_window)), __FUNCTION__);

        my_painter = painter{
            opengl_renderer{pen{color{1.f}, 1.f}, brush{texture{}, color{0.f}}, size(my_window)}};
        show(my_window);

        my_time = clock::get_time();

        set_state(game_state::paused);
        break;
    }
    case game_state::running: {
        while (my_state == game_state::running) {
            // frame variables
            pointf agent_movement = {0.f, 0.f};

            // peek frame events
            peek_events(my_event_queue, my_events);
            for (const auto& e : my_events) {
                switch (type(e)) {
                case event_type::window: {
                    if (cast<window_event>(e).my_type == window_event_type::close) {
                        my_state = game_state::stopped;
                    }
                    break;
                }
                }
                my_state =
                    (type(e) == event_type::quit) ? game_state::stopped : game_state::running;
            }

            // update logic
            const auto current_time = clock::get_time();
            float deltatime = static_cast<float>(current_time - my_time) * 0.001f;
            deltatime = std::clamp(deltatime, 0.001f, 0.33f);
            my_time = current_time;

            for (auto& logic_cmd : my_logic_commands) {
                logic_cmd(deltatime);
            }

            // render
            clear(my_painter, color(0.f, 0.f, 0.f, 1.f));

            for (auto& draw_cmd : my_draw_commands) {
                draw_cmd();
            }

            swap_buffers(my_gl_context);
        }
        break;
    }
    case game_state::paused: {
        my_logic_commands.clear();
        my_draw_commands.clear();
        break;
    }
    case game_state::stopped: {
        destroy(my_gl_context);
        destroy(my_window);

        my_video.cleanup();
        break;
    }
    }
}

void quit(game_test& game) { game.set_state(game_state::stopped); }
void pause(game_test& game) {
    game.set_state(game_state::paused);
    game.run();
}
void resume(game_test& game) {
    game.set_state(game_state::running);
    game.run();
}

void run(game_test& game) { game.run(); }

} // namespace kairos