#include <physics/force.h>
#include <algorithm>
#include "force.h"

namespace kairos {

/** Member functions */

force::force(const force& f) : is_allocated(f.is_allocated) {
    if (f.is_allocated) {
        my_self.my_impl = f.my_self.my_impl;
        my_self.my_impl->ref_count++;
    } else {
        f.get()->copy(my_self.my_memory.data());
    }
}

force& force::operator=(const force& f) {
    this->~force();
    if (f.is_allocated) {
        my_self.my_impl = f.my_self.my_impl;
        my_self.my_impl->ref_count++;
    } else {
        f.get()->copy(my_self.my_memory.data());
    }

    return *this;
}

force::force(force&& f) noexcept : is_allocated(std::move(f.is_allocated)) {
    if (f.is_allocated) {
        my_self.my_impl = std::exchange(f.my_self.my_impl, nullptr);
    } else {
        f.get()->move(my_self.my_memory.data());
        f.my_self.my_memory = {'\0'};
    }
}

force& force::operator=(force&& f) noexcept {
    this->~force();
    if (f.is_allocated) {
        my_self.my_impl = std::exchange(f.my_self.my_impl, nullptr);
    } else {
        f.get()->move(my_self.my_memory.data());
    }

    return *this;
}

force::~force() {
    if (is_allocated) {
        if (--my_self.my_impl->ref_count <= 0) {
            delete my_self.my_impl;
            my_self.my_impl = nullptr;
        }
    } else if (std::any_of(std::begin(my_self.my_memory), std::end(my_self.my_memory),
                           [](char c) { return c != '\0'; })) {
        get()->destruct();
        my_self.my_memory = {'\0'};
    }
}

force::force_t* force::get() {
    if (is_allocated) {
        return my_self.my_impl;
    } else {
        return reinterpret_cast<force_t*>(my_self.my_memory.data());
    }
}

const force::force_t* force::get() const {
    if (is_allocated) {
        return my_self.my_impl;
    } else {
        return reinterpret_cast<const force_t*>(my_self.my_memory.data());
    }
}

/** Non-member functions */

void apply_force(force& f, particle& p) {
    f.get()->update_force(p);
}

} // namespace kairos