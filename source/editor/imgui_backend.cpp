#include <imgui_backend.h>

namespace kairos {

imgui_backend::imgui_backend(const imgui_backend& other) : my_self(other.my_self->clone()) {}

imgui_backend& imgui_backend::operator=(const imgui_backend& other) {
    my_self.reset(other.my_self->clone());
    return *this;
}

imgui_backend::imgui_backend(imgui_backend&& other) noexcept : my_self(std::move(other.my_self)) {}

imgui_backend& imgui_backend::operator=(imgui_backend&& other) noexcept {
    my_self = std::move(other.my_self);
    return *this;
}

bool imgui_backend::setup(window window) { return my_self->setup(window); }

void imgui_backend::cleanup() { my_self->cleanup(); }

void imgui_backend::update() { my_self->update(); }

void imgui_backend::process_event(const system_event& event) { my_self->process_event(event); }

void show(imgui_backend& imgui) { imgui.my_self->show(); }

void hide(imgui_backend& imgui) { imgui.my_self->hide(); }

bool is_shown(const imgui_backend& imgui) { return imgui.my_self->is_shown(); }

bool is_hidden(const imgui_backend& imgui) { return imgui.my_self->is_hidden(); }

} // namespace kairos