#pragma once

#include <types.h>

template<typename Value, typename Predicate>
auto find_if(const array<Value>& array, Predicate&& predicate) {
    return std::find_if(array.begin(), array.end(), std::forward<Predicate>(predicate));
}

template<typename Value, typename Predicate>
auto find_if(array<Value>& array, Predicate&& predicate) {
    return std::find_if(array.begin(), array.end(), std::forward<Predicate>(predicate));
}

template<typename Value, typename Predicate>
auto remove_if(array<Value>& array, Predicate&& predicate) {
    return std::remove_if(array.begin(), array.end(), std::forward<Predicate>(predicate));
}

template<typename Value>
auto find(const array<Value>& array, const Value& value) {
    return std::find(array.begin(), array.end(), value);
}

template<typename Value>
auto remove(array<Value>& array, const Value& value) {
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