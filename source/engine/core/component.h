#pragma once

#include <core/hash.h>
#include <core/types.h>

class component {
public:
    static constexpr uint32 type_id = "component"_hash;

    component(uint32 id) : my_id(id) {}
    uint32 id() const { return my_id; }

private:
    const uint32 my_id;
};