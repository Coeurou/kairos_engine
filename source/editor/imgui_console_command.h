#pragma once

#include <command_history.h>
#include <core/types.h>

class imgui_console_command {
public:
    void render(bool* is_open);
private:
    bool enable_auto_scroll = true;
    static_array<char, 256> my_command_buffer;
    command_history<128> my_command_history;
};