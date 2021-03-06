#include <core/messages.h>

#include <algorithm>

#include <nameof.hpp>

#include <core/enum_utils.h>
#include <core/event_listener.h>
#include <core/formattable.h>
#include <core/logger.h>

namespace kairos {

MessageDispatcher::MessageDispatcher() {
    for (auto& listener : my_listeners) {
        listener.reserve(8);
    }
}

std::vector<EventListener*>& MessageDispatcher::operator[](size_t index) {
    return my_listeners[index];
}

void allocate_memory_for_messages() {
    for (auto& listener : message_dispatcher.my_listeners) {
        listener.reserve(8);
    }
}

void send_message(variant sender, variant recipient, MessageType type) {
    send_message(sender, recipient, type, array<variant>());
}

void send_message(variant sender, variant recipient, MessageType type,
                  const array<variant>& message_data) {
    messages.emplace(Message{sender, recipient, type, message_data});
    log(LoggerName::MESSAGE, "receive {} message from {} to {}\n", NAMEOF_ENUM(type),
        to_string(&messages.back().sender), to_string(&messages.back().recipient));
}

void subscribe(EventListener& listener, std::initializer_list<MessageType> channels) {
    for (auto channel : channels) {
        message_dispatcher[to_index(channel)].emplace_back(&listener);
        log(LoggerName::MESSAGE, "new listener added to {} channel. channel listeners count: {}\n",
            NAMEOF_ENUM(channel), message_dispatcher[to_index(channel)].size());
    }
}

void unsubscribe(EventListener& listener) {
    for (auto subscription : listener.my_subscriptions) {
        auto& channel_subscriptions = message_dispatcher[to_index(subscription)];
        channel_subscriptions.erase(
            std::remove(channel_subscriptions.begin(), channel_subscriptions.end(), &listener));
        log(LoggerName::MESSAGE, "listener removed of {} channel. channel listeners count: {}\n",
            NAMEOF_ENUM(subscription),
            message_dispatcher[to_index(subscription)].size());
    }
}

void dispatch_messages() {
    while (!messages.empty()) {
        auto& message = messages.front();
        for (auto listener : message_dispatcher[to_index(message.type)]) {
            listener->my_pending_messages.emplace(message);
        }
        messages.pop();
    }
}

} // namespace kairos