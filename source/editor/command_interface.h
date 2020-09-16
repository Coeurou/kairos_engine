#pragma once

#include <formattable.h>
#include <types.h>

class command_interface {
public:
    virtual ~command_interface() {};

    virtual string to_string() const = 0;
    virtual bool undo() = 0;
    virtual bool redo() = 0;
    virtual bool execute() = 0;
};

template<> inline string to_string(command_interface* command) {
    return command->to_string();
}


