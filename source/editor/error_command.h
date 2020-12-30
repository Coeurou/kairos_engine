#pragma once

#include <abstract_command.h>

namespace kairos {

class error_command : public abstract_command {
  public:
    error_command() = default;
    error_command(string error_message);

    static constexpr string_view name() { return "error_command"; }

    string to_string() const override;
    bool undo() override;
    bool redo() override;
    bool execute() override;

    string my_error_message = "";
};

} // namespace kairos