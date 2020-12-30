#include <core/event.h>

namespace kairos {

/** Member functions */

system_event_queue::system_event_queue(const system_event_queue& eq) {
    eq.get()->copy(my_memory.data());
}

system_event_queue& system_event_queue::operator=(const system_event_queue& eq) {
    this->~system_event_queue();
    eq.get()->copy(my_memory.data());

    return *this;
}

system_event_queue::system_event_queue(system_event_queue&& eq) noexcept {
    eq.get()->move(my_memory.data());
    eq.my_memory = {'\0'};
}

system_event_queue& system_event_queue::operator=(system_event_queue&& eq) noexcept {
    this->~system_event_queue();
    eq.get()->move(my_memory.data());

    return *this;
}

system_event_queue::~system_event_queue() {
    if (std::any_of(std::begin(my_memory), std::end(my_memory), [](char c) { return c != '\0'; })) {
        get()->destruct();
        my_memory = {'\0'};
    }
}

system_event_queue::event_queue_t* system_event_queue::get() {
    return reinterpret_cast<event_queue_t*>(my_memory.data());
}

const system_event_queue::event_queue_t* system_event_queue::get() const {
    return reinterpret_cast<const event_queue_t*>(my_memory.data());
}

/** Non-member functions */

void peek_events(system_event_queue& eq, array<system_event>& output) {
    eq.get()->peek_events(output);
}

} // namespace kairos
