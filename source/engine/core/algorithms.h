#pragma once

#include <core/types.h>

// in c++20 all this should be constexpr
template <typename Array, typename Predicate>
auto find_if(const Array& array, Predicate&& predicate) {
    return std::find_if(array.begin(), array.end(), std::forward<Predicate>(predicate));
}

template <typename Array, typename Predicate> auto find_if(Array& array, Predicate&& predicate) {
    return std::find_if(array.begin(), array.end(), std::forward<Predicate>(predicate));
}

template <typename Array, typename Predicate>
auto remove_if(Array& array, Predicate&& predicate) {
    return std::remove_if(array.begin(), array.end(), std::forward<Predicate>(predicate));
}

template <typename Array, typename Value> auto find(const Array& array, const Value& value) {
    return std::find(array.begin(), array.end(), value);
}

template <typename Array, typename Value> auto find(Array& array, const Value& value) {
    return std::find(array.begin(), array.end(), value);
}

template <typename Array, typename Value> auto remove(Array& array, const Value& value) {
    return std::remove(array.begin(), array.end(), value);
}

template <typename Container, typename Value> bool contains(Container& container, const Value& value) {
    return find(container, value) != container.end();
}

template <typename Container, typename Predicate>
bool contains(Container& container, Predicate&& predicate) {
    return find_if(container, std::forward<Predicate>(predicate)) != container.end();
}

template<typename FunctorCreation, typename FunctorDestruction>
class scoped_object {
public:
    scoped_object(FunctorCreation&& on_create, FunctorDestruction&& on_destroy) : on_destroy(on_destroy) {
        on_create();
    }

    ~scoped_object() {
        on_destroy();
    }

private:
    FunctorDestruction on_destroy;
};