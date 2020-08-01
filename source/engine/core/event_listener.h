#pragma once

#include <queue>
#include <vector>
#include <messages.h>

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

    std::vector<MessageType> subscriptions{};
    std::queue<Message> pending_messages{};
};