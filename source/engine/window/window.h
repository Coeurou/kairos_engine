#pragma once

#include <core/types.h>

namespace kairos {

/** window is a type-erased class encapsulating any implementation of a window.
 * It carries the interface every window must fulfill.
 * Window has value semantic, it is copyable and movable.
 * It implements copy on write and small buffer optimisation.
* Any class copy/move-constructible implementing as free functions id,
* size, resize, show, hide, is_shown, is_hidden, destroy,
* can be encapsulated within the window class
 */
class window {
    friend uint32 id(const window& w);
    friend vec2i size(const window& w);
    friend void resize(window& w, const vec2i& size);
    friend void show(window& w);
    friend void hide(window& w);
    friend bool is_shown(const window& window);
    friend bool is_hidden(const window& window);
    friend void destroy(window& w);

    template <class T>
    using disable_copy = std::enable_if_t<!std::is_same_v<std::decay_t<T>, window>>*;

  public:
    /** Member functions */

    window() = default;

    /** Construct a window.
    * Will do a dynamic allocation for its internal implementation, if sizeof(T) > small_type_size.
    * A small type will be constructed with placement new.
    */
    template <class T> explicit window(T&& w, disable_copy<T> = nullptr) : is_allocated(false) {
        if constexpr (sizeof(T) <= small_type_size) {
            new (my_self.memory.data()) window_impl<T>(std::forward<T>(w));
        } else {
            my_self.w = new window_impl<T>(std::forward<T>(w));
            is_allocated = true;
        }
    }

    /** Copy constructor. Use copy on write (ref_count is incremented), if sizeof(T) > small_type_size.
    * Use placement new otherwise (deep copy). */
    window(const window& w);

    /** Copy assignement operator. Use copy on write (ref_count is incremented), if sizeof(T) > small_type_size.
    * Call window_impl<T>::copy (deep copy) otherwise. */
    window& operator=(const window& w);

    /** Move constructor. Move w::window_t pointer, if sizeof(T) > small_type_size.
    * Call window_impl<T>::move otherwise. */
    window(window&& w) noexcept;

    /** Move assignement operator. Move w::window_t pointer, if sizeof(T) > small_type_size.
    * Call window_impl<T>::move otherwise. */
    window& operator=(window&& w) noexcept;

    /** Destructor. If sizeof(T) > small_type_size and no other reference of window_t pointer,
    * delete my_self.w. Call window_impl<T>::destroy otherwise */
    ~window();

  private:
    /** Type erasure details */

    /** window_t represents the window interface. */
    struct window_t {
        /** Copy window_t with placement new. */
        virtual void copy(void* memory) const = 0;
        /** Move window_t with placement new. */
        virtual void move(void* memory) noexcept = 0;
        /** Allocate a copy of window_t. */
        virtual void* clone() const = 0;
        /** Call destructor of derived class. */
        virtual void destruct() = 0;
        virtual uint32 window_id() const = 0;
        virtual vec2i window_size() const = 0;
        virtual void resize_window(const vec2i& size) = 0;
        virtual void show_window() = 0;
        virtual bool is_window_shown() const = 0;
        virtual void hide_window() = 0;
        virtual bool is_window_hidden() const = 0;
        virtual void destroy_window() = 0;
        virtual ~window_t() {}

        int ref_count = 0;
    };

    /** window_impl implements the window interface, by forwarding calls to type-erased concrete
     * window. */
    template <class T> struct window_impl final : public window_t {

        /** Constructor. Forward arguments to T::Constructor. */
        template <class... Args>
        explicit window_impl(Args&&... args) : concrete_window(std::forward<Args>(args)...) {
            static_assert(!std::is_same_v<std::decay_t<T>, window_impl>,
                          "Should use proper copy or move constructor");
            ref_count++;
        }

        /** Copy T with placement new. Call T copy constructor. */
        void copy(void* memory) const override { new (memory) window_impl<T>(concrete_window); }
        /** Move T with placement new. Call T move constructor. */
        void move(void* memory) noexcept { new (memory) window_impl<T>(std::move(concrete_window)); }
        /** Call destructor. */
        void destruct() override { this->~window_impl(); }
        /** Copy T with new dynamic allocation. */
        void* clone() const override {
            return new window_impl(concrete_window);
        }
        void destroy_window() override { destroy(concrete_window); }
        uint32 window_id() const override { return id(concrete_window); }
        vec2i window_size() const override { return size(concrete_window); }
        void resize_window(const vec2i& size) override { resize(concrete_window, size); }
        void show_window() override { show(concrete_window); }
        bool is_window_shown() const override { return is_shown(concrete_window); }
        void hide_window() override { hide(concrete_window); }
        bool is_window_hidden() const override { return is_hidden(concrete_window); }

        /** Type instance where the logic of a window is implemented */
        T concrete_window;
    };

    static constexpr size_t small_type_size = 16;
    static constexpr size_t pointer_size = 8;
    static constexpr size_t padding = 7;
    /** buffer is a union type describing either an dynamically allocated window_t
    * or a small buffer used for small buffer optimisation. */
    union buffer {
        window_t* w;
        static_array<char, small_type_size + pointer_size + padding> memory{ '\0' };
    };

    /** Return a pointer to window_t whether or not my_self is allocated. */
    window_t* get();
    const window_t* get() const;

    void detach();

    /** my_self stores all window logic */
    buffer my_self;
    /** Is my_self dynamically allocated */
    bool is_allocated = false;
};

/** Non-member functions */

/** Return the size of a window */
uint32 id(const window& w);

/** Return the size of a window */
vec2i size(const window& w);

/** Resize a window. Will do a deep copy if the window is used somewhere else (copy on write) */
void resize(window& w, const vec2i& size);

/** Show a window. Will do a deep copy if the window is used somewhere else (copy on write) */
void show(window& w);

/** Checks whether the window is shown */
bool is_shown(const window& w);

/** Hide a window. Will do a deep copy if the window is used somewhere else (copy on write) */
void hide(window& w);

/** Checks whether the window is hidden */
bool is_hidden(const window& w);

/** Destroy a window. */
void destroy(window& w);

} // namespace kairos