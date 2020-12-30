#include <imgui_renderer.h>

namespace kairos {

imgui_renderer::imgui_renderer(imgui_renderer&& other) noexcept
    : my_self(std::move(other.my_self)) {}

imgui_renderer& imgui_renderer::operator=(imgui_renderer&& other) noexcept {
    my_self = std::move(other.my_self);
    return *this;
}

bool imgui_renderer::setup() { return my_self->setup(); }

void imgui_renderer::cleanup() { my_self->cleanup(); }

void imgui_renderer::render(ImDrawData* draw_data) { my_self->render(draw_data); }

} // namespace kairos