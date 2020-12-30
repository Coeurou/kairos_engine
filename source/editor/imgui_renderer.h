#pragma once

#include <memory>

struct ImDrawData;

namespace kairos {
/** imgui_renderer is a type-erased class encapsulating any implementation of imgui backend.
 * It carries the interface every imgui backend must fulfill.
 * imgui_renderer has value semantic, it is movable not copyable since it is a renderer dedicated
 * only to imgui which is alone throughout the application. It allocates on the heap except on move. Any
 * class copy-constructible implementing as methods setup, render(ImDrawData*), cleanup and, can be
 * encapsulated within the imgui_renderer class
 */
class imgui_renderer {
  public:
    template <class T>
    imgui_renderer(T&& concrete_renderer)
        : my_self(std::make_unique<imgui_renderer_impl<T>>(std::forward<T>(concrete_renderer))) {}
    imgui_renderer(const imgui_renderer& other) = delete;
    imgui_renderer& operator=(const imgui_renderer& other) = delete;
    imgui_renderer(imgui_renderer&& other) noexcept;
    imgui_renderer& operator=(imgui_renderer&& other) noexcept;

    bool setup();
    void cleanup();
    void render(ImDrawData* draw_data);

  private:
    struct imgui_renderer_t {
        virtual ~imgui_renderer_t() {}

        virtual bool setup() = 0;
        virtual void cleanup() = 0;
        virtual void render(ImDrawData* draw_data) = 0;
    };

    template <class T> struct imgui_renderer_impl : public imgui_renderer_t {
        template <class... Args>
        explicit imgui_renderer_impl(Args&&... args)
            : concrete_renderer(std::forward<Args>(args)...) {}

        bool setup() override { return concrete_renderer.setup(); }
        void cleanup() override { concrete_renderer.cleanup(); }
        void render(ImDrawData* draw_data) override { concrete_renderer.render(draw_data); }

        T concrete_renderer;
    };

    std::unique_ptr<imgui_renderer_t> my_self = nullptr;
};
} // namespace kairos