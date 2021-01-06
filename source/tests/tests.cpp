#include <iostream>
#include <core/types.h>
#include <unit_tests/physics_tests.h>

void run_tests(string_view mode) {
    if (mode == "-u" || mode == "--unit") {
        test_particle();
    }
}

int main(int argc, char* argv[]) {
    for (auto i = 1; i < argc; i++) {
        run_tests(argv[i]);
    }

    return EXIT_SUCCESS;
}
