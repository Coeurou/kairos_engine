#pragma once

#include <queue>
#include <core/messages.h>
#include <core/types.h>

namespace kairos {

// TODO doc class is a resource with RAII for subscribe/unsubscribe to channels
class EventListener {
  public:
    EventListener();
    // TODO doc subscribe with this ctor
    EventListener(std::initializer_list<MessageType> channels);
    // TODO doc unsubscribe in dtor
    ~EventListener();

    EventListener(const EventListener&) = delete;
    EventListener& operator=(const EventListener&) = delete;

    array<MessageType> my_subscriptions{};
    std::queue<Message> my_pending_messages{};
};

} // namespace kairos