#pragma once

#include <algorithm>

#include <abstract_command.h>
#include <core/contract.h>
#include <core/types.h>

namespace kairos {

template <size_t N> class command_history {
  public:
    command_history() { my_command_logs.reserve(N); }

    int current_command_index() const noexcept { return my_current_command_index; }

    void set_current_command_index(int index) { my_current_command_index = index; }

    string at(int index) const noexcept {
        expects(index >= 0 && index < N);
        return my_command_history[index]->to_string();
    }

    array<string>::iterator begin() noexcept { return my_command_logs.begin(); }

    array<string>::const_iterator begin() const noexcept { return my_command_logs.begin(); }

    array<string>::iterator end() noexcept { return my_command_logs.end(); }

    array<string>::const_iterator end() const noexcept { return my_command_logs.end(); }

    void add(std::unique_ptr<abstract_command>&& log) {
        my_command_history[my_last_command_index] = std::move(log);
        my_command_logs.emplace_back(my_command_history[my_last_command_index]->to_string());
        my_last_command_index++;
        my_current_command_index = my_last_command_index;

        if (my_last_command_index == my_command_history.size()) {
            my_command_history[0].reset(nullptr);
            my_current_command_index = --my_last_command_index;
            // shift left
            std::rotate(my_command_history.begin(), my_command_history.begin() + 1,
                        my_command_history.end());
        }
    }

    void clear() noexcept { my_command_logs.clear(); }

    size_t size() const noexcept { return my_command_history.size(); }

  private:
    static_array<std::unique_ptr<abstract_command>, N> my_command_history;
    int my_last_command_index = 0;
    int my_current_command_index = 0;
    array<string> my_command_logs;
};

} // namespace kairos