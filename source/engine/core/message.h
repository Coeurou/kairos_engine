#pragma once

#include <core/types.h>

enum class MessageType {
    key_pressed = 0,
    mouse_button_pressed,
    window_resized,
    component_attached,
    component_detached,
    entity_erased,
    position_changed,
    puck_is_oob,
    game_started,
    fading_ended,
    game_over,
    save_world_requested,
    save_shaders_requested,
    collision_occured,
    game_mode_selected,
    main_menu_requested,
    count
};

struct Message {
    variant sender = 0;
    variant recipient = 0;
    MessageType type = MessageType::count;
    array<variant> data;
};
