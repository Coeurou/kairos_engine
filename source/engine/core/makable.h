#pragma once

#include <memory>
#include <type_traits>

template<typename Makable, class... args> std::unique_ptr<Makable> make(args&&... arguments) {
    return std::make_unique<Makable>(std::forward<args>(arguments)...);
}