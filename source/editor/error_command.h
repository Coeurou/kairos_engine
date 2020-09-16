#pragma once

#include <command_interface.h>

class error_command : public command_interface {
public:
    error_command() = default;
    error_command(string error_message);

    static constexpr string_view name() {
        return "error_command";
    }

    string to_string() const override;
    bool undo() override;
    bool redo() override;
    bool execute() override;

    string my_error_message = "";
};