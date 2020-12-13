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
    controller_axis_move,
    controller_button_press,
    controller_button_release,
    audio_device_add,
    audio_device_remove,
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
    std::pair<variant, variant> my_data = { 0, 0 };
};

struct keyboard_event {
    uint32 my_window_id = 0;
    action my_action = action::count;
    key my_key = key::KeyCount;
};

struct mouse_event {
    uint32 my_window_id = 0;
    action my_action = action::count;
    mouse_button my_button = mouse_button::last_button;
    vec2i my_mouse_pos = { 0, 0 };
};

struct controller_event {
    uint32 my_controller_id = 0;
    action my_action = action::count;

    controller_axis my_axis = controller_axis::count;
    vec2i my_axis_value = { 0, 0 };
    controller_device_state my_device_state = controller_device_state::count;
};

struct audio_event {
    uint32 my_device_id = 0;
    bool is_capture = false;
};

struct system_event {
    event_type my_type = event_type::count;
    uint32 my_timestamp = 0;
    std::variant<window_event, keyboard_event, mouse_event> my_internal;
};

/** system_event_queue is a type-erased class encapsulating any implementation of an event queue.
* system_event_queue has value semantic, it is copyable and movable.
* Any class with sizeof(T) == 0 and copy/move-constructible and implementing as free functions peek_events,
* can be encapsulated within the system_event_queue class.
*/
class system_event_queue {
    friend void peek_events(system_event_queue& eq, array<system_event>& output);

    template <class T>
    using disable_copy = std::enable_if_t<!std::is_same_v<std::decay_t<T>, system_event_queue>>*;

public:
    /** Member functions */

    /** Construct a system_event_queue. */
    template <class T> explicit system_event_queue(T&& eq, disable_copy<T> = nullptr) {
        static_assert(sizeof(T) <= small_type_size, "Event queue implementation must be empty class");
        new (my_memory.data()) event_queue_impl<T>(std::forward<T>(eq));
    }

    /** Copy constructor. Use copy on write (ref_count is incremented), if sizeof(T) > small_type_size.
    * Use placement new otherwise (deep copy). */
    system_event_queue(const system_event_queue& eq);

    /** Copy assignement operator. Use copy on write (ref_count is incremented), if sizeof(T) > small_type_size.
    * Call window_impl<T>::copy (deep copy) otherwise. */
    system_event_queue& operator=(const system_event_queue& eq);

    /** Move constructor. Move w::window_t pointer, if sizeof(T) > small_type_size.
    * Call window_impl<T>::move otherwise. */
    system_event_queue(system_event_queue&& eq) noexcept;

    /** Move assignement operator. Move w::window_t pointer, if sizeof(T) > small_type_size.
    * Call window_impl<T>::move otherwise. */
    system_event_queue& operator=(system_event_queue&& eq) noexcept;

    /** Destructor. If sizeof(T) > small_type_size and no other reference of window_t pointer,
    * delete my_self.w. Call window_impl<T>::destroy otherwise */
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

    /** event_queue_impl implements the event queue interface, by forwarding calls to type-erased concrete
    * type. */
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
        void move(void* memory) noexcept { new (memory) event_queue_impl<T>(std::move(event_queue)); }
        /** Call destructor. */
        void destruct() override { this->~event_queue_impl(); }
        void peek_events(array<system_event>& output) override { return ::kairos::peek_events(event_queue, output); }

        /** Type instance where the logic of a window is implemented */
        T event_queue;
    };

    static constexpr size_t small_type_size = 8;
    /** my_memory is a used for placement new the event_queue_t concrete type. */
    std::array<char, small_type_size> my_memory{ '\0' };
    
    event_queue_t* get();
    const event_queue_t* get() const;
};

void peek_events(system_event_queue& w, array<system_event>& output);

} // namespace kairos