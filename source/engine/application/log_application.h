#pragma once

#include <application.h>

class log_application : public application {
public:
    bool init() override;
    void exec() override;
    void exit() override;
    array<variant_event> process_events() override;
};
inline log_application our_log_application;