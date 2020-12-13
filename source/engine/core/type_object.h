#pragma once

#include <core/hash.h>
#include <core/member_object.h>
#include <core/types.h>

template <size_t N> class type_object {
  public:
    static constexpr  uint32 no_id = 0;
    type_object() = delete;

    constexpr explicit type_object(string_view name) : my_id(hash(name)) {}

    template <typename... Members>
    constexpr type_object(std::string_view name, Members&&... members)
        : my_id(hash(name)), my_members({{members...}}) {
        static_assert(N >= sizeof...(Members),
                      "Constructor has too much members parameter, adjust N template parameter");
    }
    template <size_t M> constexpr type_object(const type_object<M>& other) : my_id(other.id()) {
        static_assert(N >= M,
                      "LValue type_object parameter has more members, adjust N template parameter");
        const std::array<member_object, M>& members_to_copy = other.members();
        for (auto i = 0; i < members_to_copy.size(); i++) {
            my_members[i] = members_to_copy[i];
        }
    }
    template <size_t M>
    constexpr type_object(type_object<M>&& other) noexcept : my_id(std::move(other.my_id)) {
        static_assert(N >= other.my_members.size(),
                      "RValue type_object parameter has more members, adjust N template parameter");
        auto& members_to_move = other.members();
        for (auto i = 0; i < members_to_move.size(); i++) {
            my_members[i] = std::move(members_to_move[i]);
        }
        other.my_id = no_id;
    }

    template <size_t M> constexpr type_object& operator=(const type_object<M>& other) {
        static_assert(N >= other.my_members.size(),
                      "LValue type_object parameter has more members, adjust N template parameter");
        my_id = other.id();
        constexpr auto& members_to_copy = other.members();
        for (auto i = 0; i < members_to_copy.size(); i++) {
            my_members[i] = members_to_copy[i];
        }
        return *this;
    }
    template <size_t M> constexpr type_object& operator=(type_object<M>&& other) noexcept {
        static_assert(N >= other.my_members.size(),
                      "RValue type_object parameter has more members, adjust N template parameter");
        my_id = std::move(other.my_id);
        constexpr auto& members_to_move = other.members();
        for (auto i = 0; i < members_to_move.size(); i++) {
            my_members[i] = std::move(members_to_move[i]);
        }
        other.my_id = no_id;
        return *this;
    }

    constexpr uint32 id() const { return my_id; }

    constexpr const member_object* get_member(uint32 id) const {
        // should std::find_if but it is constexpr started from c++20
        for (auto& member : my_members) {
            if (member.id() == id) {
                return &(*member);
            }
        }
        return nullptr;
    }

    constexpr std::optional<uint32> member(string_view name) const {
        if (const auto object_member = get_member(hash(name)); object_member != nullptr) {
            return object_member->id();
        } else {
            return std::nullopt;
        }
    }

    constexpr std::array<member_object, N>& members() { return my_members; }

    constexpr const std::array<member_object, N>& members() const { return my_members; }

    constexpr uint32 parent() { return my_parent_id; }

    constexpr void set_parent(uint32 parent) { my_parent_id = parent; }

  private:
    uint32 my_parent_id = no_id;
    uint32 my_id = no_id;
    std::array<member_object, N> my_members;
};
// additional deduction guide
template <typename... Members>
type_object(std::string_view name, Members&&... members) -> type_object<sizeof...(Members)>;

template <size_t N>
constexpr bool operator==(const type_object<N>& lhs, const type_object<N>& rhs) {
    return lhs.id() == rhs.id();
}

template <size_t N> constexpr bool operator==(const type_object<N>& lhs,  uint32 id) {
    return lhs.id() == id;
}

template <size_t N> constexpr bool operator==(const type_object<N>& lhs, string_view name) {
    return lhs.id() == hash(name);
}

// With r-value, parent is ignored
template <size_t M, size_t N>
constexpr type_object<M + N> operator+(type_object<M>&& lhs, type_object<N>&& rhs) {
    type_object<M + N> res{lhs};
    auto& members = res.members();
    // should std::partition the collection but it is constexpr started from c++20
    auto available_slot = std::begin(members);
    for (; available_slot < std::end(members); available_slot++) {
        if (available_slot->id() == member_object::no_id) {
            break;
        }
    }
    const auto offset = std::distance(std::begin(members), available_slot);
    const auto& members_to_move = rhs.members();
    for (auto i = 0; i < members_to_move.size(); i++) {
        members[offset + i] = std::move(members_to_move[i]);
    }
    return res;
}

// With l-value, right hand side argument is considered as parent type
template <size_t M, size_t N>
constexpr type_object<M + N> operator+(const type_object<M>& lhs, const type_object<N>& rhs) {
    type_object<M + N> res{lhs};
    auto& members = res.members();
    // should std::partition the collection but it is constexpr started from c++20
    auto available_slot = std::begin(members);
    for (; available_slot < std::end(members); available_slot++) {
        if (available_slot->id() == member_object::no_id) {
            break;
        }
    }
    const auto offset = std::distance(std::begin(members), available_slot);
    const auto& members_to_copy = rhs.members();
    for (auto i = 0; i < members_to_copy.size(); i++) {
        members[offset + i] = members_to_copy[i];
    }
    res.set_parent(rhs.id());
    return res;
}

template <typename T> constexpr uint32 type_id(T&& /*obj*/) {
    return T::type_descriptor.id(); 
}

template <typename T, typename U> constexpr bool is_instance_of(T&& /*obj*/) {
    return (T::type_descriptor == U::type_descriptor) || (U::type_descriptor == T::type_descriptor.parent());
}

template <typename T> constexpr bool has_member(T&& /*obj*/, string_view member_name) {
    return T::type_descriptor.member(member_name).has_value();
}

template <typename T> std::string name(T&& /*obj*/) {
    return T::name;
}
