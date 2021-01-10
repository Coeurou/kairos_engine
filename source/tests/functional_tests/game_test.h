#pragma once

#include <functional>

#include <core/event.h>
#include <core/types.h>
#include <graphics/opengl/opengl_context.h>
#include <graphics/painter.h>
#include <graphics/service/video_service.h>
#include <window/window.h>

namespace kairos {

enum class game_state { not_initialized, running, paused, stopped, terminated };

using update_func = std::function<void(float)>;
using draw_func = std::function<void()>;

class game_test {
  public:
    game_test();

    void add_logic_command(update_func cmd);
    void add_draw_command(draw_func cmd);
    void set_state(game_state state);
    painter get_painter() const;
    window get_window() const;
    void run();

  private:
    game_state my_state = game_state::not_initialized;
    array<update_func> my_logic_commands;
    array<draw_func> my_draw_commands;
    video_service my_video;
    opengl_context my_gl_context;
    window my_window;
    painter my_painter;
    double my_time = 0.0;

    system_event_queue my_event_queue;
    array<system_event> my_events;
};

void quit(game_test& game);
void pause(game_test& game);
void resume(game_test& game);
void run(game_test& game);

} // namespace kairos