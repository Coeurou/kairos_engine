#pragma once

#include <type_traits>

#include <formattable.h>
#include <json.h>
#include <types.h>

template<typename T>
struct is_serialization_strategy {
    static const bool value = false;
};

template<>
struct is_serialization_strategy<json> {
    static const bool value = true;
};

template<>
struct is_serialization_strategy<string> {
    static const bool value = true;
};

template<>
struct is_serialization_strategy<array<std::byte>> {
    static const bool value = true;
};

template<class T>
inline constexpr bool is_serialization_strategy_v = is_serialization_strategy<T>::value;

template<typename T, typename std::enable_if_t<is_serialization_strategy_v<T>>>
struct serialization_strategy {
    template<typename Serializable> void serialize(T& object, Serializable&& serializable) {
        if constexpr (std::is_same_v<T, json>) {
            to_json(object, std::forward<Serializable>(serializable));
        }
        else if constexpr (std::is_same_v<T, string>) {
            object = to_string(std::forward<Serializable>(serializable));
        }
        else if constexpr (std::is_same_v<T, array<std::byte>>) {
            log_error(LoggerName::MISC, "NOT IMPLEMENTED for std::byte\n");
        }
        else {
            static_assert(false, "non-exhaustive visitor!");
        }
    }

    template<typename Serializable> void deserialize(const T& object, Serializable& serializable) {
        if constexpr (std::is_same_v<T, json>) {
            from_json(object, serializable);
        }
        else if constexpr (std::is_same_v<T, string>) {
            serializable = from_string(object);
        }
        else if constexpr (std::is_same_v<T, array<std::byte>>) {
            log(LoggerName::MISC, "NOT IMPLEMENTED for std::byte\n");
        }
        else {
            static_assert(false, "non-exhaustive visitor!");
        }
    }
};