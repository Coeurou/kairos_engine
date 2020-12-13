#pragma once

#include <utility>
#include <variant>
#include <core/types.h>

enum class error_type { invalid_parameter, runtime_error, no_error, invalid_result, uninitialized };

template <typename T> class result {
  public:
    constexpr result() noexcept : my_result(error_type::invalid_result) {}
    constexpr explicit result(T&& value) noexcept : my_result(std::forward<T>(value)) {}
    constexpr result(error_type error) noexcept : my_result(error) {}
    template <class... Args>
    constexpr explicit result(Args&&... args)
        : my_result(std::in_place_type<T>, std::forward<Args>(args)...) {}
    constexpr result(const result& other) : my_result(other.my_result) {}
    constexpr result(result&& other) noexcept : my_result(std::move(other.my_result)) {}

    constexpr result& operator=(const result& rhs) {
        my_result = rhs.my_result;
        return *this;
    }
    constexpr result& operator=(result&& rhs) noexcept {
        my_result = std::move(rhs.my_result);
        return *this;
    }
    result& operator=(T&& t) noexcept {
        my_result = std::forward<T>(t);
        return *this;
    }

    constexpr const T& operator*() const& { return std::get<value_index>(my_result); }
    constexpr T& operator*() & { return std::get<value_index>(my_result); }
    constexpr const T&& operator*() const&& { return std::get<value_index>(my_result); }
    constexpr T&& operator*() && { return std::get<value_index>(my_result); }

    constexpr T& value() & { return std::get<value_index>(my_result); }
    constexpr const T& value() const& { return std::get<value_index>(my_result); }
    constexpr T&& value()&& { return std::get<value_index>(my_result); }
    constexpr const T&& value() const&& { return std::get<value_index>(my_result); }

    constexpr error_type error() const noexcept {
        if (my_result.index() == error_index) {
            return std::get<error_index>(my_result);
        } else {
            return error_type::no_error;
        }
    }

    constexpr bool has_value() const noexcept { return my_result.index() == value_index; }

    constexpr explicit operator bool() const noexcept { return has_value(); }

  private:
    static constexpr uint32 value_index = 1;
    static constexpr uint32 error_index = 0;
    std::variant<error_type, T> my_result;
};
