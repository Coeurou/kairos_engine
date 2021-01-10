#include <unit_tests/physics_tests.h>

#include <physics/particle.h>
#include <core.h>

using namespace kairos;

std::pair<uint32, uint32> test_particle() {
    constexpr uint32 nb_tests = 13;
    uint32 nb_tests_passed = 0;

    /* particle is default constructible */
    {
        particle p;

        bool is_initialized = p.position() == real_vec2(0.f) && p.velocity() == real_vec2(0.f) &&
                              p.applied_forces() == real_vec2(0.f) && p.inverse_mass() == 0.f &&
                              p.damping() != 0.f;
        nb_tests_passed += to_integer(is_initialized);
        log_unit_test_result("particle_is_default_constructible", is_initialized);
    }

    /* particle is copy constructible */
    constexpr real_vec2 expected_pos{1.f, 1.f};
    constexpr real_vec2 expected_vel{2.f, 2.f};
    constexpr real_vec2 expected_force{3.f, 3.f};
    constexpr real expected_mass{4.f};
    constexpr real expected_damping{0.5f};

    {
        particle p(expected_pos, expected_vel, expected_mass, expected_damping);
        p.apply_force(expected_force);

        particle copy(p);

        bool is_equal = (p == copy);
        nb_tests_passed += to_integer(is_equal);
        log_unit_test_result("particle_is_copy_constructible", is_equal);
    }

    /* particle is move constructible */
    {
        particle moved(particle(expected_pos, expected_vel, expected_mass, expected_damping));

        bool has_expected_properties =
            moved.position() == expected_pos && moved.velocity() == expected_vel &&
            moved.mass() == expected_mass && moved.damping() == expected_damping;
        nb_tests_passed += to_integer(has_expected_properties);
        log_unit_test_result("particle_is_move_constructible", has_expected_properties);
    }

    /* particle can be assigned with l-value */
    {
        particle p(expected_pos, expected_vel, expected_mass, expected_damping);
        p.apply_force(expected_force);

        particle copy;
        copy = p;

        bool is_equal = (p == copy);
        nb_tests_passed += to_integer(is_equal);
        log_unit_test_result("particle_l_value_assignment", is_equal);
    }

    /* particle can be assigned with r-value */
    {
        particle moved;
        moved = particle(expected_pos, expected_vel, expected_mass, expected_damping);

        bool has_expected_properties =
            moved.position() == expected_pos && moved.velocity() == expected_vel &&
            moved.mass() == expected_mass && moved.damping() == expected_damping;
        nb_tests_passed += to_integer(has_expected_properties);
        log_unit_test_result("particle_r_value_assignment", has_expected_properties);
    }

    /* forces can be applied on a particle */
    {
        constexpr real_vec2 expected_forces{4.f, -9.81f};
        particle p;

        p.apply_force(real_vec2(4.f, 0.f));
        apply_gravity(p, -9.81f);

        bool has_force_applied = (p.applied_forces() == expected_forces);
        nb_tests_passed += to_integer(has_force_applied);
        log_unit_test_result("particle_apply_forces", has_force_applied);
    }

    /* forces can be cleared on a particle */
    {
        real_vec2 expected_forces{4.f, -9.81f};
        particle p;
        p.apply_force(real_vec2(4.f, 0.f));
        apply_gravity(p, -9.81f);
        bool has_force_applied = (p.applied_forces() == expected_forces);

        p.clear_applied_force();

        has_force_applied &= p.applied_forces() == real_vec2(0.f);
        nb_tests_passed += to_integer(has_force_applied);
        log_unit_test_result("particle_clear_forces", has_force_applied);
    }

    /* the relation between mass and inverse mass is invariant */
    {
        particle p;
        constexpr real mass = 4.f;

        p.set_mass(mass);

        bool has_expected_mass = (p.mass() == mass && p.inverse_mass() == 1.f / mass);
        nb_tests_passed += to_integer(has_expected_mass);
        log_unit_test_result("particle_set_mass", has_expected_mass);
    }
    {
        particle p;
        constexpr real inv_mass = 0.5f;

        p.set_inverse_mass(inv_mass);

        bool has_expected_mass = (p.inverse_mass() == inv_mass && p.mass() == 1.f / inv_mass);
        nb_tests_passed += to_integer(has_expected_mass);
        log_unit_test_result("particle_set_inverse_mass", has_expected_mass);
    }

    /* integrate a moving particle (velocity != 0) modify its position */
    {
        real_vec2 default_pos = real_vec2(0.f);
        particle p(default_pos, real_vec2(1.f));

        bool has_moved = true;
        constexpr auto count = 10;
        for (auto i = 0; i < count; i++) {
            integrate(p, 1.f / 60.f);

            if (!(p.position() != default_pos && p.position().x > 0.f)) {
                log(LoggerName::MISC, "[particle_position_integration] Integration test failure at iteration {}", i);
                has_moved = false;
                break;
            }
        }

        nb_tests_passed += to_integer(has_moved);
        log_unit_test_result("particle_position_integration", has_moved);
    }

    /* integrate an accelerating particle modify its velocity */
    {
        real_vec2 default_pos = real_vec2(0.f);
        real_vec2 default_vel = real_vec2(0.f);
        particle p(default_pos, default_vel, 0.01f);

        apply_gravity(p, -9.81f);

        bool is_moving = true;
        constexpr auto count = 10;

        for (auto i = 0; i < count; i++) {
            integrate(p, 1.f / 60.f);

            if (!(p.velocity() != default_vel && p.velocity().y < 0.f)) {
                log(LoggerName::MISC, "[particle_velocity_integration] Integration test failure at iteration {}", i);
                is_moving = false;
                break;
            }
        }

        nb_tests_passed += to_integer(is_moving);
        log_unit_test_result("particle_velocity_integration", is_moving);
    }

    /* after integrating an accelerating particle applied forces are cleared */
    {
        particle p(real_vec2(0.f), real_vec2(0.f));

        p.apply_force(real_vec2(15.f, 0.f));
        apply_gravity(p, -9.81f);

        bool has_force_applied = (p.applied_forces().x != 0.f && p.applied_forces().y != 0.f);
        log_unit_test_result("particle_force_integration_1_2", has_force_applied);

        constexpr auto count = 10;
        for (auto i = 0; i < count; i++) {
            integrate(p, 1.f / 60.f);

            has_force_applied = (p.applied_forces().x != 0.f || p.applied_forces().y != 0.f);
            if (has_force_applied) {
                log(LoggerName::MISC, "[particle_force_integration] Integration test failure at iteration {}", i);
                break;
            }
        }

        nb_tests_passed += to_integer(!has_force_applied);
        log_unit_test_result("particle_force_integration_2_2", !has_force_applied);
    }

    /* each integration particle velocity is modified by damping */
    {
        real_vec2 velocity = real_vec2(1.f);
        particle p(real_vec2(0.f), velocity, 1.f, 0.5f);

        bool is_damping_applied = true;
        constexpr auto count = 10;
        for (auto i = 0; i < count; i++) {
            velocity = p.velocity();

            integrate(p, 1.f / 60.f);

            if (!(p.velocity().x < velocity.x && p.velocity().y < velocity.y)) {
                log(LoggerName::MISC, "[particle_damping] Integration test failure at iteration {}", i);
                is_damping_applied = false;
                break;
            }
        }

        nb_tests_passed += to_integer(is_damping_applied);
        log_unit_test_result("particle_damping", is_damping_applied);
    }

    return {nb_tests_passed, nb_tests};
}
