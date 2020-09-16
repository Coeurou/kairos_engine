#pragma once

#include <serialization_strategy.h>

template<typename T, typename std::enable_if_t<is_serialization_strategy_v<T>>>
struct serializer {
    template<typename Serializable> void serialize(T& object, Serializable&& serializable) {
        my_strategy.serialize(object, std::forward<Serializable>(serializable));
    }

    template<typename Serializable> void deserialize(const T& object, Serializable& serializable) {
        my_strategy.deserialize(object, serializable);
    }

	serialization_strategy<T> my_strategy;
};