#pragma once

#include <application/application.h>

class sdl_application : public application {
    bool init() override;
    void exec() override;
    void exit() override;
    array<variant_event> process_events() override;
};
inline sdl_application our_sdl_application;
