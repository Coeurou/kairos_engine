#pragma once

#include <sdl_application.h>
#include <chrono>
#include <random>

#include <imgui.h>

#include <contract.h>
#include <painter.h>
#include <transform.h>

bool sdl_application::init() {
    my_type = implementation_type::SDL;
    expects(SDL_Init(SDL_INIT_VIDEO) >= 0, fmt::format("Couldn't initilize SDL: {}", SDL_GetError()));
    return true;
}

void sdl_application::exec() {
    texture::set_implementation(renderer_engine::opengl);
    texture t;
    t.setup("../../assets/icon_large.png");
    texture t1;
    t1.setup("../../assets/grass.png");
    t.bind();
    t1.bind();

    painter p(renderer_engine::opengl);
    p.set_line_width(4.f);
    p.set_brush_color(color{1.f});
    p.draw_lines({ linef{ {100.f, 100.f}, {200.4f, 200.5f} }, linef{ {400.f, 400.f}, {200.4f, 0.5f} } });
    p.draw_lines({ { linef{ {400.f, 700.f}, {1800.4f, 900.5f} }, color{ 1.f, 0.8f, 0.3f, 1.f } },
                   { linef{ {1400.f, 600.f}, {40.4f, 900.5f} }, color{ 0.f, 0.8f, 0.8f, 1.f } } });
    p.set_line_width(1.f);
    p.set_pen_color(color(0.214f, 0.742f, 0.963f, 1.0f));
    p.draw_rect(rectf{{500.f, 500.f}, {700.f, 850.f}});
    p.set_brush_texture(t.my_index);
    p.draw_rect(rectf{{900.f, 300.f}, {1412.f, 812.f}});
    p.set_brush_color(color{1.f, 0.f, 0.f, 1.f});
    p.set_line_width(10.2f);
    p.draw_line(linef{{1200.f, 800.f}, {200.4f, 900.5f}});
    //p.set_line_width(0.2f);
    p.set_brush_color(color{0.6f, 0.f, 1.f, 1.f});
    p.set_brush_texture(-1);
    p.draw_rects({
        rectf{{1600.f, 700.f}, {1650.f, 800.f}},
        rectf{{1500.f, 500.f}, {1750.f, 550.f}},
        rectf{{1800.f, 100.f}, {1850.f, 150.f}},
    });
    p.draw_rects({
        {rectf{{1600.f, 200.f}, {1650.f, 300.f}}, color(0.1f, 0.6f, 0.1f, 1.f)},
        {rectf{{1800.f, 900.f}, {1850.f, 950.f}}, color(0.1f, 0.2f, 0.7f, 1.f)},
    });
    p.draw_rects({
        {rectf{{600.f, 200.f}, {750.f, 300.f}}, t1},
        {rectf{{800.f, 50.f}, {850.f, 150.f}}, t},
    });

    rectf bounds{{100.f, 500.f}, {300.f, 700.f}};
    sprite grass_sprite{&t1, bounds, rect<int>{{0, 0}, t1.my_size},
                        color{1.f}};
    
    grass_sprite.set_origin(grass_sprite.my_bounds.center());
    transform<sprite> tr;
    grass_sprite.my_rotation = tr.rotate(grass_sprite, std::sin(frame_number) * 360.f, grass_sprite.my_origin);
    p.draw_sprite(grass_sprite);

    t.cleanup();
    t1.cleanup();
    frame_number++;
}

void sdl_application::exit() {
    SDL_Quit();
    my_gl_context.reset(nullptr);
}

array<variant_event> sdl_application::process_events() {
    static constexpr int event_queue_empty = 0;
    SDL_Event e;
    array<variant_event> polled_events{};
    polled_events.reserve(4);
    while (SDL_PollEvent(&e) != event_queue_empty) {
        switch (e.type) {
        case SDL_QUIT: {
            my_main_window->set_should_close(true);
            break;
        }
        case SDL_MOUSEBUTTONDOWN: {
            break;
        }
        case SDL_MOUSEBUTTONUP: {
            break;
        }
        case SDL_KEYDOWN: {
            my_main_window->set_should_close(e.key.keysym.scancode == SDL_SCANCODE_ESCAPE);
            break;
        }
        case SDL_KEYUP: {
            break;
        }
        case SDL_WINDOWEVENT_RESIZED: {
            break;
        }
        }
        polled_events.emplace_back(e);
    }
    return polled_events;
}
