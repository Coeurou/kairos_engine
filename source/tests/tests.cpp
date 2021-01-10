#include <iostream>

#include <core/types.h>
#include <core.h>
#include <unit_tests/physics_tests.h>
#include <functional_tests/physics_functional_tests.h>

void run_tests(string_view mode) {
    if (mode == "-u" || mode == "--unit") {
        print_test_results("particle_unit", test_particle());
    }
    if (mode == "-f" || mode == "--func") {
        run(test_game);
        //print_test_results("particle_ballistic", test_ballistic());
        print_test_results("particle_firework", test_firework());
        quit(test_game);
    }
}

int main(int argc, char* argv[]) {
    for (auto i = 1; i < argc; i++) {
        run_tests(argv[i]);
    }

    return EXIT_SUCCESS;
}
