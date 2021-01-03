#pragma once

#include <core/types.h>
#include <core/format.h>
#include <core/formattable.h>

namespace kairos {

struct window_params {
    static constexpr float our_undefined_pos = -1.f;
    string my_title{""};
    sizef my_size{0.f, 0.f};
    pointf my_pos{our_undefined_pos, our_undefined_pos};
    uint32 my_flags = 0;
};

inline string to_string(const window_params& params) {
    return format("size: {}, pos: {}, title: {}", to_string(params.my_size),
                  to_string(params.my_pos), params.my_title);
}

} // namespace kairos