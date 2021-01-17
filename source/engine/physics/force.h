#pragma once

#include <core/types.h>

namespace kairos {

class particle;

/** type-erased class for force abstraction. a force can be applied on one or several particles. the
 * class is copy and move constructible. it will allocate memory if erased type size is more than 8
 * bytes */
class force {
    friend uint32 apply_force(force& f, particle& p);

  public:
    force() = default;

    template <class T> force(T&& force) {
        if constexpr (sizeof(T) <= small_type_size) {
            new (my_self.my_memory) force_generator<T>(std::forward<T>(force));
        } else {
            my_self.my_impl = new force_generator<T>(std::forward<T>(force));
        }
    }

    /** Copy constructor. Use copy on write (ref_count is incremented), if sizeof(T) >
     * small_type_size. Use placement new otherwise (deep copy). */
    force(const force& f);

    /** Copy assignement operator. Use copy on write (ref_count is incremented), if sizeof(T) >
     * small_type_size. Call force_generator<T>::copy (deep copy) otherwise. */
    force& operator=(const force& f);

    /** Move constructor. Move my_impl::force_t pointer, if sizeof(T) > small_type_size.
     * Call force_generator<T>::move otherwise. */
    force(force&& f) noexcept;

    /** Move assignement operator. Move my_impl::force_t pointer, if sizeof(T) > small_type_size.
     * Call force_generator<T>::move otherwise. */
    force& operator=(force&& f) noexcept;

    /** Destructor. If sizeof(T) > small_type_size and no other reference of force_t pointer,
     * delete my_self.my_impl. Call force_generator<T>::destroy otherwise */
    ~force();

  private:
    static constexpr size_t small_type_size = 8;
    static constexpr size_t pointer_size = 8;
    static constexpr size_t padding = 7;

    struct force_t {
        /** Copy window_t with placement new. */
        virtual void copy(void* memory) const = 0;
        /** Move window_t with placement new. */
        virtual void move(void* memory) noexcept = 0;
        /** Allocate a copy of window_t. */
        virtual void* clone() const = 0;
        /** Call destructor of derived class. */
        virtual void destruct() = 0;
        virtual void update_force(particle& p) = 0;
        virtual ~force_t() {}

        int ref_count = 0;
    };

    template <class T> class force_generator : public force_t {
      public:
        template <class... Args>
        explicit force_generator(Args&&... args) : my_force(std::forward<Args>(args)...) {
            static_assert(!std::is_same_v<std::decay_t<T>, force_generator>,
                          "Should use proper copy or move constructor");
        }

        /** Copy T with placement new. Call T copy constructor. */
        void copy(void* memory) const override { new (memory) force_generator<T>(my_force); }
        /** Move T with placement new. Call T move constructor. */
        void move(void* memory) noexcept { new (memory) force_generator<T>(std::move(my_force)); }
        /** Call destructor. */
        void destruct() override { this->~force_generator(); }
        /** Copy T with new dynamic allocation. */
        void* clone() const override { return new force_generator(my_force); }
        void update_force(particle& p) override { apply_force(my_force, p); }

      private:
        /** Type instance where the logic of force is implemented */
        T my_force{};
    };

    union buffer {
        force_t* my_impl;
        static_array<char, small_type_size + pointer_size + padding> my_memory = {'\0'};
    };
    buffer my_self;
    bool is_allocated = false;

    force_t* get();
    const force_t* get() const;
};

/** Non-member functions */

void apply_force(force& f, particle& p);

} // namespace kairos