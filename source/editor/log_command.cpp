#pragma once

#include <log_command.h>

#include <core/logger.h>

namespace kairos {

string log_command::to_string() const { return name().data(); }

bool log_command::undo() {
    log(LoggerName::EDITOR, "Undo a log command");
    return true;
}

bool log_command::redo() {
    log(LoggerName::EDITOR, "Redo a log command");
    return true;
}

bool log_command::execute() {
    log(LoggerName::EDITOR, "Executing a log command");
    return true;
}

} // namespace kairos
