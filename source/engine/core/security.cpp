#include <core/security.h>

#include <cstdlib>
#include <filesystem>
#include <fstream>

namespace kairos {

static const std::filesystem::path cmd_output_filename{"processes.txt"};

bool is_unique_instance(string_view program_name) {
    if (std::filesystem::exists(cmd_output_filename)) {
        std::filesystem::remove(cmd_output_filename);
    }

    std::string executable_name{program_name};
    const auto is_running_multiple_instances = [&]() {
        if (std::ifstream is{cmd_output_filename}) {
            std::string file_content{std::istreambuf_iterator<char>(is),
                                     std::istreambuf_iterator<char>()};
            return file_content.find(executable_name) ==
                   file_content.rfind(executable_name);
        }
        else {
            return false;
        }
    };

    if (std::system(fmt::format("tasklist > {}", cmd_output_filename.string()).c_str()) == 0) {
        executable_name = executable_name.substr(executable_name.find_last_of('\\')+1);
        return is_running_multiple_instances();
    } else if (std::system(fmt::format("pgrep -l {} > {}", program_name.data(),
                                       cmd_output_filename.string())
                               .c_str()) == 0) {
        return is_running_multiple_instances();
    }
    else {
        return true;
    }
}

} // namespace kairos;