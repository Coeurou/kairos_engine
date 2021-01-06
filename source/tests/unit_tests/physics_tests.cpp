#include <unit_tests/physics_tests.h>

#include <core/logger.h>
#include <physics/particle.h>

using namespace kairos;

uint32 to_integer(bool condition) { return (condition) ? 1 : 0; }

void log_test_result(const string& test_name, bool condition) {
    log(LoggerName::MISC, "[UNIT_TEST] {} : {}\n", test_name, (condition) ? "PASSED" : "FAIL");
}

std::pair<uint32, uint32> test_particle() {
    constexpr uint32 nb_tests = 5;
    uint32 nb_tests_passed = 0;

    /* particle is default constructible */
    {
        particle p;
        bool is_initialized = p.position() == real_vec2(0.f) && p.velocity() == real_vec2(0.f) &&
                              p.applied_forces() == real_vec2(0.f) && p.inverse_mass() == 0.f &&
                              p.damping() != 0.f;
        nb_tests_passed += to_integer(is_initialized);
        log_test_result("particle_is_default_constructible", is_initialized);
    }

    /* particle is copy constructible */
    real_vec2 expected_pos{1.f, 1.f};
    real_vec2 expected_vel{2.f, 2.f};
    real_vec2 expected_force{3.f, 3.f};
    real expected_mass{4.f};
    real expected_damping{0.5f};

    {
        particle p(expected_pos, expected_vel, expected_mass, expected_damping);
        p.apply_force(expected_force);

        particle copy(p);
        bool is_equal = (p == copy);
        nb_tests_passed += to_integer(is_equal);
        log_test_result("particle_is_copy_constructible", is_equal);
    }

    /* particle is move constructible */
    {
        particle moved(particle(expected_pos, expected_vel, expected_mass, expected_damping));
        bool has_expected_properties =
            moved.position() == expected_pos && moved.velocity() == expected_vel &&
            moved.mass() == expected_mass && moved.damping() == expected_damping;
        nb_tests_passed += to_integer(has_expected_properties);
        log_test_result("particle_is_move_constructible", has_expected_properties);
    }

    /* particle can be assigned with l-value */
    {
        particle p(expected_pos, expected_vel, expected_mass, expected_damping);
        p.apply_force(expected_force);

        particle copy;
        copy = p;
        bool is_equal = (p == copy);
        nb_tests_passed += to_integer(is_equal);
        log_test_result("particle_l_value_assignment", is_equal);
    }

    /* particle can be assigned with r-value */
    {
        particle moved;
        moved = particle(expected_pos, expected_vel, expected_mass, expected_damping);
        bool has_expected_properties =
            moved.position() == expected_pos && moved.velocity() == expected_vel &&
            moved.mass() == expected_mass && moved.damping() == expected_damping;
        nb_tests_passed += to_integer(has_expected_properties);
        log_test_result("particle_r_value_assignment", has_expected_properties);
    }

    return {nb_tests_passed, nb_tests};
}
