#pragma once

#include <core/types.h>
#include <input/controller.h>
#include <input/keyboard.h>
#include <input/input.h>
#include <input/mouse.h>

namespace kairos {

enum class event_type {
    window,
    key_press,
    key_release,
    mouse_move,
    mouse_button_press,
    mouse_button_release,
    mouse_wheel,
    controller_device,
    controller_axis_move,
    controller_button_press,
    controller_button_release,
    audio_device_add,
    audio_device_remove,
    text_input,
    quit,
    sysWM_event,
    count
};

enum class window_event_type {
    show,
    hide,
    move,
    resize,
    minimize,
    maximize,
    restore,
    mouse_enter,
    mouse_leave,
    keyboard_gain_focus,
    keyboard_lose_focus,
    close,
    count
};

struct window_event {
    uint32 my_window_id = 0;
    window_event_type my_type = window_event_type::count;
    std::pair<variant, variant> my_data = {0, 0};
};

struct keyboard_event {
    uint32 my_window_id = 0;
    action my_action = action::count;
    key my_key = key::count;
};

struct mouse_event {
    uint32 my_window_id = 0;
    action my_action = action::count;
    mouse_button my_button = mouse_button::last_button;
    vec2i my_mouse_pos = {0, 0};
};

struct mouse_wheel_event {
    uint32 my_window_id = 0;
    bool is_flipped = false;
    vec2i my_scroll = {0, 0};
};

struct controller_event {
    uint32 my_controller_id = 0;
    action my_action = action::count;
    controller_button my_button = controller_button::count;

    controller_axis my_axis = controller_axis::count;
    int my_axis_value = 0;
    controller_device_state my_device_state = controller_device_state::count;
};

struct text_input_event {
    uint32 my_window_id = 0;
    string my_text = "";
};

struct audio_event {
    uint32 my_device_id = 0;
    bool is_capture = false;
};

struct system_event {
    event_type my_type = event_type::count;
    uint32 my_timestamp = 0;
    std::variant<window_event, keyboard_event, mouse_event, mouse_wheel_event, text_input_event, controller_event>
        my_internal;
};

constexpr event_type type(const system_event& e) noexcept {
    return e.my_type;
}

template <class SubType> constexpr const SubType& cast(const system_event& output) {
    return std::get<SubType>(output.my_internal);
}

/** system_event_queue is a type-erased class encapsulating any implementation of an event queue.
 * system_event_queue has value semantic, it is copyable and movable.
 * Any class with sizeof(T) == 0 and copy/move-constructible and implementing as free functions
 * peek_events, can be encapsulated within the system_event_queue class.
 */
class system_event_queue {
    friend void peek_events(system_event_queue& eq, array<system_event>& output);

    template <class T>
    using disable_copy = std::enable_if_t<!std::is_same_v<std::decay_t<T>, system_event_queue>>*;

  public:
    /** Member functions */

    /** Construct a system_event_queue. */
    template <class T> explicit system_event_queue(T&& eq, disable_copy<T> = nullptr) {
        static_assert(sizeof(T) <= small_type_size,
                      "Event queue implementation must be empty class");
        new (my_memory.data()) event_queue_impl<T>(std::forward<T>(eq));
    }

    /** Copy constructor. Copy concrete event_queue_t type. Invoke copy method. */
    system_event_queue(const system_event_queue& eq);

    /** Copy assignement operator. Copy concrete event_queue_t type. Invoke copy method. */
    system_event_queue& operator=(const system_event_queue& eq);

    /** Move constructor. Move concrete event_queue_t type. Invoke move method. */
    system_event_queue(system_event_queue&& eq) noexcept;

    /** Move assignement operator. Move concrete event_queue_t type. Invoke move method. */
    system_event_queue& operator=(system_event_queue&& eq) noexcept;

    /** Destructor. Call destroy method on concrete event_queue_t type. Set memory to \0. */
    ~system_event_queue();

  private:
    /** Type erasure details */

    /** event_queue_t represents the event queue interface. */
    struct event_queue_t {

        /** Copy event_queue_t with placement new. */
        virtual void copy(void* memory) const = 0;
        /** Move event_queue_t with placement new. */
        virtual void move(void* memory) noexcept = 0;
        /** Call destructor of derived class. */
        virtual void destruct() = 0;
        virtual void peek_events(array<system_event>& output) = 0;
        virtual ~event_queue_t() {}
    };

    /** event_queue_impl implements the event queue interface, by forwarding calls to type-erased
     * concrete type. */
    template <class T> struct event_queue_impl final : public event_queue_t {

        /** Constructor. Forward arguments to T::Constructor. */
        template <class... Args>
        explicit event_queue_impl(Args&&... args) : event_queue(std::forward<Args>(args)...) {
            static_assert(!std::is_same_v<std::decay_t<T>, event_queue_impl>,
                          "Should use proper copy or move constructor");
        }

        /** Copy T with placement new. Call T copy constructor. */
        void copy(void* memory) const override { new (memory) event_queue_impl<T>(event_queue); }
        /** Move T with placement new. Call T move constructor. */
        void move(void* memory) noexcept {
            new (memory) event_queue_impl<T>(std::move(event_queue));
        }
        /** Call destructor. */
        void destruct() override { this->~event_queue_impl(); }
        void peek_events(array<system_event>& output) override {
            return ::kairos::peek_events(event_queue, output);
        }

        /** Type instance where the logic of an event queue is implemented */
        T event_queue;
    };

    static constexpr size_t small_type_size = 8;
    /** my_memory is a used for placement new the event_queue_t concrete type. */
    std::array<char, small_type_size> my_memory{'\0'};

    event_queue_t* get();
    const event_queue_t* get() const;
};

/** Non-member functions */

void peek_events(system_event_queue& w, array<system_event>& output);

} // namespace kairos