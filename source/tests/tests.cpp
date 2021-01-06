#include <iostream>
#include <core/types.h>
#include <unit_tests/physics_tests.h>

void print_test_results(const string& test_category,
                        const std::pair<uint32, uint32>& test_results) {
    string res = (test_results.first == test_results.second) ? "SUCCESS" : "FAILURE";
    std::cout << "[" << test_category << "] - " << test_results.first << "/"
              << test_results.second << " -> " << res << std::endl;
}

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
