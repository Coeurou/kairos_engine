#pragma once

#include <type_traits>

#include <core/contract.h>
#include <core/component.h>
#include <core/types.h>

enum class error_code {
    invalid_argument,
    no_error
};

namespace Data {

namespace detail {
template <typename ComponentType,
          typename = std::enable_if_t<std::is_base_of_v<component, ComponentType>>>
array<uint32> to_ids(const array<ComponentType>& components) {
    array<uint32> ids;
    ids.reserve(components.size());

    std::transform(std::begin(components), std::end(components), std::back_inserter(ids),
                   [](const ComponentType& component) { return component.my_id; });
    return ids;
}

template <typename... Args> struct components_filter;

template <typename ComponentType, typename... Args> struct components_filter<ComponentType, Args...> {
    static array<uint32> apply(const array<uint32>& ids) {
        const auto& components = all<ComponentType>();
        array<uint32> sorted_ids = to_ids(components);        
        std::sort(std::begin(sorted_ids), std::end(sorted_ids));

        array<uint32> intersection;
        intersection.reserve(std::min(ids.size(), sorted_ids.size()));
        std::set_intersection(std::begin(ids), std::end(ids), std::begin(sorted_ids),
                              std::end(sorted_ids), std::back_inserter(intersection));
        return components_filter<Args...>::apply(intersection);
    }
};

template <> struct components_filter<> {
    static array<uint32> apply(const array<uint32>& ids) {
        return ids;
    }
};
} // namespace detail

template <typename ComponentType,
          typename = std::enable_if_t<std::is_base_of_v<component, ComponentType>>>
const array<ComponentType>& all() {
    static_assert(false, "all is partially implemented for each concrete component");
    return array<ComponentType>();
}
template <typename ComponentType,
          typename = std::enable_if_t<std::is_base_of_v<component, ComponentType>>>
array<ComponentType>& all() {
    static_assert(false, "all is partially implemented for each concrete component");
    return array<ComponentType>();
}

template <typename ComponentType,
          typename = std::enable_if_t<std::is_base_of_v<component, ComponentType>>>
std::pair<ComponentType&, error_code> find(uint32 id) {
    const auto& components = all<ComponentType>();
    const auto found_component =
        std::find_if(std::begin(components), std::end(components),
                     [&id](const ComponentType& component) { component.my_id == id; });
    error_code error = (found_component != std::end(components)) ? error_code::no_error : error_code::invalid_argument;
    return {*found_component, error};
}

template <typename ComponentType,
          typename = std::enable_if_t<std::is_base_of_v<component, ComponentType>>>
 void add(ComponentType&& component) {
    auto& components = all<ComponentType>();
    components.emplace_back(std::forward(component));
}

template <typename ComponentType,
          typename = std::enable_if_t<std::is_base_of_v<component, ComponentType>>>
void add(std::initializer_list<ComponentType&&> components_to_add) {
    auto& components = all<ComponentType>();
    for (auto&& component : components_to_add) {
        components.emplace_back(std::forward(component));
    }
}

template <typename ComponentType,
          typename = std::enable_if_t<std::is_base_of_v<component, ComponentType>>>
void remove(uint32 id) {
    auto& components = all<ComponentType>();
    components.erase(std::remove_if(std::begin(components), std::end(components),
                     [&id](const ComponentType& component) { component.my_id == id; }));
}

template <typename ComponentType,
          typename = std::enable_if_t<std::is_base_of_v<component, ComponentType>>>
void remove(std::initializer_list<uint32> ids) {
    auto& components = all<ComponentType>();
    for (auto id : ids) {
        components.erase(
            std::remove_if(std::begin(components), std::end(components),
                           [&id](const ComponentType& component) { component.my_id == id; }));
    }
}

template <typename... Args> array<uint32> view() {
    return detail::filter::apply<Args...>();
}

} // namespace Data