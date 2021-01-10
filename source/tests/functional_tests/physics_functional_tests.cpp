#include <functional_tests/physics_functional_tests.h>

#include <algorithm>

#include <core.h>
#include <core/formattable.h>
#include <core/random.h>
#include <functional_tests/game_test.h>

using namespace kairos;

std::pair<uint32, uint32> test_ballistic() {
    constexpr uint32 nb_tests = 4;
    uint32 nb_tests_passed = 0;

    constexpr float test_duration_in_seconds = 3.f;
    const auto start_pos = real_vec2(0.f, 300.f);
    /** bullet test */
    {
        particle bullet(start_pos, real_vec2(35.f, 0.f), 2.f);

        float duration = 0.f;
        test_game.add_logic_command([&](float dt) {
            duration += dt;
            if (duration < test_duration_in_seconds) {
                apply_gravity(bullet, -1.f);
                integrate(bullet, dt);
            } else {
                pause(test_game);
            }
        });
        test_game.add_draw_command([&]() {
            auto bullet_pos = bullet.position();
            bullet_pos.y = size(test_game.get_window()).y - bullet_pos.y;
            auto p = test_game.get_painter();

            draw(p, rectf{{bullet_pos - pointf{5.f}}, {bullet_pos + pointf{5.f}}});
        });
        resume(test_game);
        bool has_moved = bullet.position().x > start_pos.x && bullet.position().y < start_pos.y;
        nb_tests_passed += to_integer(has_moved);
        log_func_test_result("bullet", has_moved);
    }

    /** mortar test */
    {
        particle mortar(start_pos, real_vec2(20.f, 12.f), 200.f);

        float duration = 0.f;
        test_game.add_logic_command([&](float dt) {
            duration += dt;
            if (duration < test_duration_in_seconds) {
                apply_gravity(mortar, -500.f);
                integrate(mortar, dt);
            } else {
                pause(test_game);
            }
        });
        test_game.add_draw_command([&]() {
            auto mortar_pos = mortar.position();
            mortar_pos.y = size(test_game.get_window()).y - mortar_pos.y;
            auto p = test_game.get_painter();

            draw(p, rectf{{mortar_pos - pointf{5.f}}, {mortar_pos + pointf{5.f}}});
        });
        resume(test_game);
        bool has_moved = mortar.position().x > start_pos.x;
        nb_tests_passed += to_integer(has_moved);
        log_func_test_result("mortar", has_moved);
    }

    /** fireball test */
    {
        particle fireball(start_pos, real_vec2(10.f, 0.f), 200.f);

        float duration = 0.f;
        test_game.add_logic_command([&](float dt) {
            duration += dt;
            if (duration < test_duration_in_seconds) {
                apply_gravity(fireball, 0.6f);
                integrate(fireball, dt);
            } else {
                pause(test_game);
            }
        });
        test_game.add_draw_command([&]() {
            auto fireball_pos = fireball.position();
            fireball_pos.y = size(test_game.get_window()).y - fireball_pos.y;
            auto p = test_game.get_painter();

            draw(p, rectf{{fireball_pos - pointf{5.f}}, {fireball_pos + pointf{5.f}}});
        });
        resume(test_game);
        bool has_moved = fireball.position().x > start_pos.x && fireball.position().y > start_pos.y;
        nb_tests_passed += to_integer(has_moved);
        log_func_test_result("fireball", has_moved);
    }

    /** laser test */
    {
        particle laser(start_pos, real_vec2(100.f, 0.f), 0.1f);

        float duration = 0.f;
        test_game.add_logic_command([&](float dt) {
            duration += dt;
            if (duration < test_duration_in_seconds) {
                integrate(laser, dt);
            } else {
                pause(test_game);
            }
        });
        test_game.add_draw_command([&]() {
            auto laser_pos = laser.position();
            laser_pos.y = size(test_game.get_window()).y - laser_pos.y;
            auto p = test_game.get_painter();

            draw(p, rectf{{laser_pos - pointf{5.f}}, {laser_pos + pointf{5.f}}});
        });
        resume(test_game);
        bool has_moved = laser.position().x > start_pos.x && laser.position().y == start_pos.y;
        nb_tests_passed += to_integer(has_moved);
        log_func_test_result("laser", has_moved);
    }

    return {nb_tests_passed, nb_tests};
}

namespace kairos {

void update(firework& f, time_span dt) {
    integrate(f.my_rocket, dt);
    f.my_age -= dt;
}

void explode(firework& f, array<firework>& fireworks, const explosion_params& params) {
    if (f.my_type == firework::type::parent) {
        real_vec2 random_velocity = f.my_rocket.velocity();
        const uint32 children_count =
            random_int(params.my_children_count.first, params.my_children_count.second);
        for (uint32 i = 0; i < children_count; i++) {
            random_velocity.x = random_real(params.my_horizontal_velocity_range.first,
                                            params.my_horizontal_velocity_range.second);
            random_velocity.y += random_real(-5.f, 5.f);

            fireworks.emplace_back(
                firework{particle{f.my_rocket.position(), random_velocity, 1.f, 0.8f},
                         random_real(params.my_age_range.first, params.my_age_range.second),
                         firework::type::child});
        }
    }
}

} // namespace kairos

std::pair<uint32, uint32> test_firework() {
    constexpr uint32 nb_tests = 1;
    uint32 nb_tests_passed = 0;

    constexpr float test_duration_in_seconds = 8.f;
    /** firework test */
    {
        array<firework> fireworks;
        constexpr auto count = 10;
        for (auto i = 0; i < count; i++) {
            fireworks.emplace_back(
                firework{ particle{real_vec2(400.f, 300.f), real_vec2(0.4f, random_real(20.f, 40.f)), 1.f},
                         random_real(2.f, 5.f), firework::type::parent });
        }
        explosion_params params{};

        float duration = 0.f;
        test_game.add_logic_command([&](float dt) {
            duration += dt;
            if (duration < test_duration_in_seconds) {
                for (auto& firework : fireworks) {
                    apply_gravity(firework.my_rocket, -1.f);
                    update(firework, dt);
                    if (firework.my_age < 0.f) {
                        explode(firework, fireworks, params);
                    }
                }
                for (auto& f : fireworks) {
                    if (f.my_age < 0.f) {
                        fireworks.erase(std::remove(std::begin(fireworks), std::end(fireworks), f));
                    }
                }

            } else {
                pause(test_game);
            }
        });
        test_game.add_draw_command([&]() {
            auto p = test_game.get_painter();
            real_vec2 firework_pos(0.f);
            for (const auto& firework : fireworks) {
                firework_pos = firework.my_rocket.position();
                firework_pos.y = size(test_game.get_window()).y - firework_pos.y;

                draw(p, rectf{{firework_pos - pointf{5.f}}, {firework_pos + pointf{5.f}}});
            }
        });
        resume(test_game);

        bool has_succeed = fireworks.capacity() > count;
        nb_tests_passed += to_integer(has_succeed);
        log_func_test_result("firework", has_succeed);
    }

    return {nb_tests_passed, nb_tests};
}
