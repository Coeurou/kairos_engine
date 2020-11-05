#pragma once

#include <core/types.h>

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

template<typename FunctorCreation, typename FunctorDestruction>
class scoped_object {
public:
    scoped_object(FunctorCreation&& on_create, FunctorDestruction&& on_destroy) : on_destroy(on_destroy) {
        on_create();
    }

    ~scoped_object() {
        this->on_destroy();
    }

private:
    FunctorDestruction on_destroy;
};