#include <iostream>

#include <core/types.h>
#include <core.h>
#include <unit_tests/physics_tests.h>

void run_tests(string_view mode) {
    if (mode == "-u" || mode == "--unit") {
        print_test_results("particle", test_particle());
    }
}

int main(int argc, char* argv[]) {
    for (auto i = 1; i < argc; i++) {
        run_tests(argv[i]);
    }

    return EXIT_SUCCESS;
}
