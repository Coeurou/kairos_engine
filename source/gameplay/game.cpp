#pragma once

#include <game.h>

#include <core/renderer_engine.h>
#include <core/time.h>
#include <graphics/painter.h>
#include <math/transform.h>

void game::run() {
    // should be in setup
    /*add_render_function([]() {
        texture::set_implementation(renderer_engine::opengl);
        texture t;
        t.setup("../../assets/spaceship.png");
        texture t1;
        t1.setup("../../assets/grass.png");
        t.bind();
        t1.bind();

        painter p(sizef(600.f, 480.f), renderer_engine::opengl);

        rectf bounds{{500.f, 500.f}, {560.f, 560.f}};
        sprite grass_sprite{&t, bounds, rect<int>{{40, 0}, {80, 40}}, color{1.f}};

        grass_sprite.set_origin(grass_sprite.my_bounds.center());
        transform<sprite> tr;
        grass_sprite.my_rotation =
            tr.rotate(grass_sprite, std::sinf(static_cast<float>(kairos::frame_number())) * 360.f,
                      grass_sprite.my_origin);
        p.draw_sprite(grass_sprite);

        t.cleanup();
        t1.cleanup();

        return result<uint32>(EXIT_SUCCESS);
    });*/
    my_main_loop.run();
}

void game::add_start_frame_function(std::function<void()>&& f) {
    my_main_loop.my_start_frame_functions.emplace_back(f);
}

void game::add_end_frame_function(std::function<void()>&& f) {
    my_main_loop.my_end_frame_functions.emplace_back(f);
}

void game::add_update_function(std::function<result<uint32>()>&& f) {
    my_main_loop.my_update_functions.emplace_back(f);
}

void game::add_render_function(std::function<result<uint32>()>&& f) {
    my_main_loop.my_render_functions.emplace_back(f);
}

result<uint32> main_loop::run() {
    double t = 0.0;
    const double dt = 0.01;

    // current_time() from facade
    double currentTime = 0.0 /* facade::current_time()*/;
    double accumulator = 0.0;

    while (/*facade::should_quit*/true) {
        double newTime = 0.0/* facade::current_time()*/;
        double frameTime = newTime - currentTime;
        currentTime = newTime;

        accumulator += frameTime;

        while (accumulator >= dt) {
            for (auto& update_function : my_update_functions) {
                if (auto update_result = update_function(); !update_result.has_value()) {
                    return update_result;
                }
            }
            accumulator -= dt;
            t += dt;
        }

        for (auto& start_frame_function : my_start_frame_functions) {
            start_frame_function();
        }
        for (auto& render_function : my_render_functions) {
            if (auto render_result = render_function(); !render_result.has_value()) {
                return render_result;
            }
        }
        for (auto& end_frame_function : my_end_frame_functions) {
            end_frame_function();
        }
    }
}