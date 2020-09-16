#pragma once

#include <formattable.h>

class texture {
public:
    uint32 id{ 0 };
};

template<> inline string to_string(texture t) {
    return fmt::format("Texture: {}", t.id);
}