#pragma once

#include <queue>

#include <core/enum_utils.h>
#include <core/message.h>
#include <core/types.h>

static constexpr int no_sender = 0;
static constexpr int no_recipient = 0;

namespace kairos {

class EventListener;

inline std::queue<Message> messages;

class MessageDispatcher {
  public:
    MessageDispatcher();
    array<EventListener*>& operator[](size_t index);
    static_array<array<EventListener*>, to_index(MessageType::count)> my_listeners;
};
inline MessageDispatcher message_dispatcher = MessageDispatcher();

void allocate_memory_for_messages();

void send_message(variant sender, variant recipient, MessageType type);

void send_message(variant sender, variant recipient, MessageType type,
                  const array<variant>& message_data);

void subscribe(EventListener& listener, std::initializer_list<MessageType> channels);

void unsubscribe(EventListener& listener);

void dispatch_messages();

template <class System> void peek_messages(System& system) {
    while (!system.my_listener.pending_messages.empty()) {
        system.handle_message(system.my_listener.pending_messages.front());
        system.my_listener.pending_messages.pop();
    }
}

} // namespace kairos
