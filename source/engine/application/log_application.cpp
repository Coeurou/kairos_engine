#pragma once

#include <log_application.h>

#include <cstdio>

#include <logger.h>
#include <sdl_window.h>

bool log_application::init() {
    my_type = implementation_type::NO_IMPL;
    log(LoggerName::SYSTEM, "Basic log application has been initialized");
    return true;
}

void log_application::exec() {
    log(LoggerName::SYSTEM, "Basic log application is running...");
}

void log_application::exit() {
    log(LoggerName::SYSTEM, "Basic log application has been exited");
}

array<variant_event> log_application::process_events() {
    auto c = std::getchar();
    log(LoggerName::SYSTEM, "Event processed pressed {}", c);
    my_main_window->set_should_close(c == static_cast<int>(key::Escape));

    return array<variant_event>();
}