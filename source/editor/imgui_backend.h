#pragma once

#include <window/window.h>

namespace kairos {

struct system_event;

/** imgui_backend is a type-erased class encapsulating any implementation of imgui backend.
 * It carries the interface every imgui backend must fulfill.
 * imgui_backend has value semantic, it is movable, not copyable since imgui as only one backend at
 * the time. It allocates on the heap except on move. Any class copy-constructible implementing as
 * methods setup(window), update, cleanup and process_events(const system_event&), can be
 * encapsulated within the imgui_backend class
 */
class imgui_backend {
    friend void show(imgui_backend& imgui);
    friend void hide(imgui_backend& imgui);
    friend bool is_shown(const imgui_backend& imgui);
    friend bool is_hidden(const imgui_backend& imgui);

  public:
    template <class T>
    imgui_backend(T&& concrete_backend)
        : my_self(std::make_unique<imgui_backend_impl<T>>(std::forward<T>(concrete_backend))) {}
    imgui_backend(const imgui_backend& other);
    imgui_backend& operator=(const imgui_backend& other);
    imgui_backend(imgui_backend&& other) noexcept;
    imgui_backend& operator=(imgui_backend&& other) noexcept;

    bool setup(window window);
    void cleanup();
    void update();
    void process_event(const system_event& event);

  private:
    struct imgui_backend_t {
        virtual ~imgui_backend_t() {}

        virtual imgui_backend_t* clone() = 0;
        virtual bool setup(window window) = 0;
        virtual void cleanup() = 0;
        virtual void update() = 0;
        virtual void process_event(const system_event& event) = 0;
        virtual void show() = 0;
        virtual bool is_shown() const = 0;
        virtual void hide() = 0;
        virtual bool is_hidden() const = 0;
    };

    template <class T> struct imgui_backend_impl : public imgui_backend_t {
        template <class... Args>
        explicit imgui_backend_impl(Args&&... args)
            : concrete_backend(std::forward<Args>(args)...) {}

        imgui_backend_t* clone() override { return new imgui_backend_impl(concrete_backend); }
        bool setup(window window) override { return concrete_backend.setup(std::move(window)); }
        void cleanup() override { concrete_backend.cleanup(); }
        void update() override { concrete_backend.update(); }
        void process_event(const system_event& event) { concrete_backend.process_event(event); }
        void show() override { ::kairos::show(concrete_backend); }
        bool is_shown() const override { return kairos::is_shown(concrete_backend); }
        void hide() override { ::kairos::hide(concrete_backend); }
        bool is_hidden() const override { return kairos::is_hidden(concrete_backend); }

        T concrete_backend;
    };

    std::unique_ptr<imgui_backend_t> my_self = nullptr;
};

void show(imgui_backend& imgui);
void hide(imgui_backend& imgui);
bool is_shown(const imgui_backend& imgui);
bool is_hidden(const imgui_backend& imgui);

} // namespace kairos