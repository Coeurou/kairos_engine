#include <event_listener.h>

static constexpr size_t subscription_reservation = 4;

EventListener::EventListener() { 
    my_subscriptions.reserve(subscription_reservation); 
}

EventListener::EventListener(std::initializer_list<MessageType> channels) : EventListener() {
    subscribe(*this, channels);
}

EventListener::~EventListener() { unsubscribe(*this); }