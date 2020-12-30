#pragma once

#include <memory>
#include <abstract_command.h>
#include <core/types.h>

namespace kairos {

class command_parser {
  public:
    static std::unique_ptr<abstract_command> parse(const string& command);
    static array<string_view> autocomplete(const string& command);

  private:
    static array<string_view> our_command_names;
};

} // namespace kairos