#pragma once

#include <core/game.h>

#include <graphics/painter.h>
#include <math/transform.h>

void game::update() {
    texture::set_implementation(renderer_engine::opengl);
    texture t;
    t.setup("../../assets/spaceship.png");
    texture t1;
    t1.setup("../../assets/grass.png");
    t.bind();
    t1.bind();

    painter p(renderer_engine::opengl);

    rectf bounds{{500.f, 500.f}, {560.f, 560.f}};
    sprite grass_sprite{&t, bounds, rect<int>{{40, 0}, {80, 40}}, color{1.f}};

    grass_sprite.set_origin(grass_sprite.my_bounds.center());
    transform<sprite> tr;
    grass_sprite.my_rotation =
        tr.rotate(grass_sprite, std::sin(frame_number) * 360.f, grass_sprite.my_origin);
    p.draw_sprite(grass_sprite);

    t.cleanup();
    t1.cleanup();
}
