#pragma once

#include <error_command.h>

#include <core/logger.h>

namespace kairos {

error_command::error_command(string error_message) : my_error_message(error_message) {}

string error_command::to_string() const { return my_error_message; }

bool error_command::undo() { return true; }

bool error_command::redo() { return true; }

bool error_command::execute() {
    log_error(LoggerName::EDITOR, my_error_message);
    return true;
}

} // namespace kairos
