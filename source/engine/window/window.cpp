#include <window/window.h>
#include <algorithm>

namespace kairos {

/** Member functions */

window::window(const window& w) : is_allocated(w.is_allocated) {
    if (w.is_allocated) {
        my_self.w = w.my_self.w;
        my_self.w->ref_count++;
    } else {
        w.get()->copy(my_self.memory.data());
    }
}

window& window::operator=(const window& w) {
    this->~window();
    if (w.is_allocated) {
        my_self.w = w.my_self.w;
        my_self.w->ref_count++;
    } else {
        w.get()->copy(my_self.memory.data());
    }

    return *this;
}

window::window(window&& w) noexcept : is_allocated(std::move(w.is_allocated)) {
    if (w.is_allocated) {
        my_self.w = std::exchange(w.my_self.w, nullptr);
    } else {
        w.get()->move(my_self.memory.data());
        w.my_self.memory = { '\0' };
    }
}

window& window::operator=(window&& w) noexcept {
    this->~window();
    if (w.is_allocated) {
        my_self.w = std::exchange(w.my_self.w, nullptr);
    } else {
        w.get()->move(my_self.memory.data());
    }

    return *this;
}

window::~window() {
    if (is_allocated) {
        if (--my_self.w->ref_count <= 0) {
            delete my_self.w;
            my_self.w = nullptr;
        }
    }
    else if (std::any_of(std::begin(my_self.memory), std::end(my_self.memory), [](char c) { return c != '\0'; })) {
        get()->destruct();
        my_self.memory = { '\0' };
    }
}

window::window_t* window::get() {
    if (is_allocated) {
        return my_self.w;
    } else {
        return reinterpret_cast<window_t*>(my_self.memory.data());
    }
}

const window::window_t* window::get() const {
    if (is_allocated) {
        return my_self.w;
    } else {
        return reinterpret_cast<const window_t*>(my_self.memory.data());
    }
}

void window::detach() {
    if (is_allocated && my_self.w->ref_count > 1) {
        my_self.w = static_cast<window_t*>(my_self.w->clone());
        my_self.w->ref_count = 1;
    }
}

/** Non-member functions */

void show(window& w) { /*w.detach();*/
    w.get()->show_window();
}

void hide(window& w) { /*w.detach();*/
    w.get()->hide_window();
}

uint32 id(const window& w) { return w.get()->window_id(); }

vec2i size(const window& w) { return w.get()->window_size(); }

void resize(window& w, const vec2i& size) { /*w.detach();*/
    w.get()->resize_window(size);
}

bool is_shown(const window& w) { return w.get()->is_window_shown(); }

bool is_hidden(const window& w) { return w.get()->is_window_hidden(); }

void destroy(window& w) {
    w.get()->destroy_window();
}

} // namespace kairos