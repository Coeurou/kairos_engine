#pragma once

#include <filesystem>

using path = std::filesystem::path;

namespace filesystem {

    inline path absolute_path(const path& path) {
        return std::filesystem::absolute(path);
    }

    inline bool exists(const path& path) {
        return std::filesystem::exists(path);
    }

    inline path current_path() {
        return std::filesystem::current_path();
    }
}