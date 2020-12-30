#pragma once

#include <command_parser.h>

#include <regex>

#include <error_command.h>
#include <log_command.h>
#include <core/makable.h>

namespace kairos {

array<string_view> command_parser::our_command_names = {log_command::name(), error_command::name()};

std::unique_ptr<abstract_command> command_parser::parse(const string& command) {
    const auto command_type = command.substr(0, command.find_first_of(' ') - 1);
    if (command_type == log_command::name()) {
        return make<log_command>(from_string<log_command>(command));
    } else {
        return make<error_command>(fmt::format("[error] Couldn't parse user command {}", command));
    }
}

array<string_view> command_parser::autocomplete(const string& command) {
    string substr = command;
    if (size_t space_pos = command.find(' '); space_pos != string::npos) {
        substr = command.substr(0, space_pos - 1);
    }
    std::regex rgx("^" + substr);

    array<string_view> candidates;
    for (const auto& name : our_command_names) {
        if (std::regex_search(name.data(), rgx)) {
            candidates.emplace_back(name);
        }
    }

    return candidates;
}

} // namespace kairos