#pragma once

#include <formattable.h>
#include <types.h>

class abstract_command {
public:
    virtual ~abstract_command() {};

    virtual string to_string() const = 0;
    virtual bool undo() = 0;
    virtual bool redo() = 0;
    virtual bool execute() = 0;
};

template<> inline string to_string(abstract_command* command) {
    return command->to_string();
}


