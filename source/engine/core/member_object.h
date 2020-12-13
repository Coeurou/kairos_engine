#pragma once

#include <core/types.h>
#include <core/hash.h>

class member_object {
  public:
    static constexpr uint32 no_id = 0;
    static constexpr uint32 empty_size = 0;
    constexpr member_object() : my_id(no_id), my_size(0), my_type_id(no_id) {}
    constexpr member_object(std::string_view name, uint32 size, uint32 type_id)
        : my_id(hash(name)), my_size(size), my_type_id(type_id) {}
    constexpr member_object(const member_object& other)
        : my_id(other.my_id), my_size(other.my_size), my_type_id(other.my_type_id) {}
    constexpr member_object& operator=(const member_object& other) {
        my_id = other.my_id;
        my_size = other.my_size;
        my_type_id = other.my_type_id;
        return *this;
    }
    constexpr member_object(member_object&& other) noexcept
        : my_id(std::move(other.my_id)), my_size(std::move(other.my_size)),
          my_type_id(std::move(other.my_type_id)) {}
    constexpr member_object& operator=(member_object&& other) noexcept {
        my_id = std::move(other.my_id);
        my_size = std::move(other.my_size);
        my_type_id = std::move(other.my_type_id);
        other.my_id = no_id;
        other.my_size = empty_size;
        other.my_type_id = no_id;
        return *this;
    }

    constexpr uint32 id() const { return my_id; }
    constexpr uint32 size() const { return my_size; }
    constexpr uint32 type_id() const { return my_type_id; }

  private:
    uint32 my_id = no_id;
    uint32 my_size = empty_size;
    uint32 my_type_id = no_id;
};
