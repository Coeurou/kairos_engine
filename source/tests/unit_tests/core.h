#pragma once

#include <iostream>
#include <core/logger.h>
#include <core/types.h>

inline void print_test_results(const string& test_category,
    const std::pair<uint32, uint32>& test_results) {
    string res = (test_results.first == test_results.second) ? "SUCCESS" : "FAILURE";
    std::cout << "[" << test_category << "] - " << test_results.first << "/"
        << test_results.second << " -> " << res << std::endl;
}

constexpr uint32 to_integer(bool condition) { return (condition) ? 1 : 0; }

inline void log_test_result(const string& test_name, bool condition) {
    log(LoggerName::MISC, "[UNIT_TEST] {} : {}\n", test_name, (condition) ? "PASSED" : "FAIL");
}