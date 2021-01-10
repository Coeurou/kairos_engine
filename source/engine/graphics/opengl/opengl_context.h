#pragma once

#include <glad/glad.h>

#include <core/types.h>

namespace kairos {

enum class gl_attribute {
    red_size,
    green_size,
    blue_size,
    alpha_size,
    framebuffer_size,
    double_buffering,
    depth_size,
    stencil_size,
    accum_red_size,
    accum_green_size,
    accum_blue_size,
    accum_alpha_size,
    stereo,
    multisample_buffers,
    aa_samples,
    hardware_acc,
    major_version,
    minor_version,
    context_profile,
    count
};

class opengl_context {
    friend bool setup(opengl_context& gl_context, uint32 window_id);
    friend void swap_buffers(opengl_context& gl_context);
    friend int version(const opengl_context& gl_context);
    friend int get_attribute(const opengl_context& gl_context, gl_attribute attr);
    friend void set_attribute(opengl_context& gl_context, gl_attribute attr, int value);
    friend void destroy(opengl_context& gl_context);

    template <class T>
    using disable_copy = std::enable_if_t<!std::is_same_v<std::decay_t<T>, opengl_context>>*;

  public:
    /** Member functions */

    opengl_context() = default;

    /** Construct an opengl_context.
     * This constructor accept any type where sizeof(T) < small_type_size, and implements the
     * interface with free functions.
     */
    template <class T> explicit opengl_context(T&& w, disable_copy<T> = nullptr) {
        static_assert(sizeof(T) <= small_type_size,
                      "opengl context implementation must have a size below small_type_size");
        new (my_memory.data()) opengl_context_impl<T>(std::forward<T>(w));
    }

    /** Copy constructor. Copy concrete opengl_context_t type. Invoke copy method. */
    opengl_context(const opengl_context& gl);

    /** Copy assignement operator. Copy concrete opengl_context_t type. Invoke copy method. */
    opengl_context& operator=(const opengl_context& gl);

    /** Move constructor. Move concrete opengl_context_t type. Invoke move method. */
    opengl_context(opengl_context&& gl) noexcept;

    /** Move assignement operator. Move concrete opengl_context_t type. Invoke move method. */
    opengl_context& operator=(opengl_context&& gl) noexcept;

    /** Destructor. Call destroy method on concrete opengl_context_t type. Set memory to \0. */
    ~opengl_context();

    enum class opengl_profile : uint8 { core, compat, es };

  private:
    struct opengl_context_t {
        virtual ~opengl_context_t() {}
        /** Copy opengl_context_t with placement new. */
        virtual void copy(void* memory) const = 0;
        /** Move opengl_context_t with placement new. */
        virtual void move(void* memory) noexcept = 0;
        /** Call destructor of derived class. */
        virtual void destruct() = 0;
        virtual bool setup_context(uint32 window_id) = 0;
        virtual void swap_gl_buffers() = 0;
        virtual int gl_version() const = 0;
        virtual int get_gl_attribute(gl_attribute attr) const = 0;
        virtual void set_gl_attribute(gl_attribute attr, int value) = 0;
        virtual void destroy_context() = 0;
    };

    /** opengl_context_impl implements the context interface, by forwarding calls to type-erased
     * concrete type. */
    template <class T> struct opengl_context_impl final : public opengl_context_t {

        /** Constructor. Forward arguments to T::Constructor. */
        template <class... Args>
        explicit opengl_context_impl(Args&&... args) : gl_context(std::forward<Args>(args)...) {
            static_assert(!std::is_same_v<std::decay_t<T>, opengl_context_impl>,
                          "Should use proper copy or move constructor");
        }

        /** Copy T with placement new. Call T copy constructor. */
        void copy(void* memory) const override { new (memory) opengl_context_impl<T>(gl_context); }
        /** Move T with placement new. Call T move constructor. */
        void move(void* memory) noexcept {
            new (memory) opengl_context_impl<T>(std::move(gl_context));
        }
        /** Call destructor. */
        void destruct() override { this->~opengl_context_impl(); }
        bool setup_context(uint32 window_id) override { return setup(gl_context, window_id); }
        void swap_gl_buffers() override { swap_buffers(gl_context); }
        int gl_version() const override { return version(gl_context); }
        int get_gl_attribute(gl_attribute attr) const { return get_attribute(gl_context, attr); }
        void set_gl_attribute(gl_attribute attr, int value) override {
            set_attribute(gl_context, attr, value);
        }
        void destroy_context() override { destroy(gl_context); }

        /** Type instance where the logic of an opengl context is implemented */
        T gl_context;
    };

    static constexpr size_t small_type_size = 24;
    /** my_memory is a used for placement new the opengl_context_t concrete type. */
    std::array<char, small_type_size> my_memory{'\0'};

    opengl_context_t* get();
    const opengl_context_t* get() const;
};

/** Non-member functions */

bool setup(opengl_context& gl_context, uint32 window_id);
void swap_buffers(opengl_context& gl_context);
int version(const opengl_context& gl_context);
int get_attribute(const opengl_context& gl_context, gl_attribute attr);
void set_attribute(opengl_context& gl_context, gl_attribute attr, int value);
void destroy(opengl_context& gl_context);

/** Utils */

namespace {
static const dictionary<int, string> gl_error_to_string = {
    {GL_NO_ERROR, "No error."},
    {GL_INVALID_ENUM, "an enumeration parameter is not a legal enumeration for that function."},
    {GL_INVALID_VALUE, "a value parameter is not a legal value."},
    {GL_INVALID_OPERATION,
     "the set of state for a command is not legal for the parameters given to that command."},
    {GL_OUT_OF_MEMORY, "an operation had allocated memory, and the memory cannot be allocated. The "
                       "results of OpenGL functions that return this error are undefined."},
    {GL_INVALID_FRAMEBUFFER_OPERATION,
     "attempt to read from or write/render to a framebuffer that is not complete."},
    /*{GL_STACK_OVERFLOW, "a stack pushing operation cannot be done because it would overflow the
    limit of that stack's size."}, {GL_STACK_UNDERFLOW, "a stack popping operation cannot be done
    because the stack is already at its lowest point."}, {GL_CONTEXT_LOST, "the OpenGL context has
    been lost, due to a graphics card reset."},*/
};
}

void check_gl_error(const char* caller);

} // namespace kairos