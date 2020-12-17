#pragma once

#include <abstract_command.h>

namespace kairos {

class log_command : public abstract_command {
  public:
    static constexpr string_view name() { return "log_command"; }

    string to_string() const override;
    bool undo() override;
    bool redo() override;
    bool execute() override;
};

template <> inline log_command from_string(string s) { return log_command{}; }

} // namespace kairos