#include <functional_tests/physics_functional_tests.h>

#include <physics/particle.h>
#include <core.h>
#include <core/formattable.h>
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
            }
            else {
                pause(test_game);
            }
            });
        test_game.add_draw_command([&]() {
            auto bullet_pos = bullet.position();
            bullet_pos.y = size(test_game.get_window()).y - bullet_pos.y;
            auto p = test_game.get_painter();

            draw(p, rectf{ {bullet_pos - pointf{5.f}}, {bullet_pos + pointf{5.f}} });
            });
        resume(test_game);
        bool has_moved = bullet.position().x > start_pos.x && bullet.position().y < start_pos.y;
        nb_tests_passed += to_integer(has_moved);
        log_func_test_result("bullet", has_moved);
    }

    /** mortar test */
    {
        particle mortar(start_pos, real_vec2(40.f, 20.f), 200.f);

        float duration = 0.f;
        test_game.add_logic_command([&](float dt) {
            duration += dt;
            if (duration < test_duration_in_seconds) {
                apply_gravity(mortar, -200.f);
                integrate(mortar, dt);
            }
            else {
                pause(test_game);
            }
            });
        test_game.add_draw_command([&]() {
            auto mortar_pos = mortar.position();
            mortar_pos.y = size(test_game.get_window()).y - mortar_pos.y;
            auto p = test_game.get_painter();

            draw(p, rectf{ {mortar_pos - pointf{5.f}}, {mortar_pos + pointf{5.f}} });
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
            }
            else {
                pause(test_game);
            }
            });
        test_game.add_draw_command([&]() {
            auto fireball_pos = fireball.position();
            fireball_pos.y = size(test_game.get_window()).y - fireball_pos.y;
            auto p = test_game.get_painter();

            draw(p, rectf{ {fireball_pos - pointf{5.f}}, {fireball_pos + pointf{5.f}} });
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
            }
            else {
                pause(test_game);
            }
            });
        test_game.add_draw_command([&]() {
            auto laser_pos = laser.position();
            laser_pos.y = size(test_game.get_window()).y - laser_pos.y;
            auto p = test_game.get_painter();

            draw(p, rectf{ {laser_pos - pointf{5.f}}, {laser_pos + pointf{5.f}} });
            });
        resume(test_game);
        bool has_moved = laser.position().x > start_pos.x && laser.position().y == start_pos.y;
        nb_tests_passed += to_integer(has_moved);
        log_func_test_result("laser", has_moved);
    }

    return {nb_tests_passed, nb_tests};
}
