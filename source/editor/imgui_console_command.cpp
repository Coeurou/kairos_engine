#pragma once

#include <imgui_console_command.h>

#include <imgui.h>
#include <command_parser.h>

void imgui_console_command::render(bool* is_open) {
    ImGui::SetNextWindowSize(ImVec2(550, 680), ImGuiCond_FirstUseEver);
    if (ImGui::Begin("Commander", is_open, ImGuiWindowFlags_NoScrollbar)) {
        // Right-Click title bar
        if (ImGui::BeginPopupContextItem()) {
            if (ImGui::MenuItem("Close")) {
                *is_open = false;
            }
            ImGui::EndPopup();
        }

        // Options
        if (ImGui::BeginPopup("Options")) {
            ImGui::Checkbox("Auto-scroll", &enable_auto_scroll);
            ImGui::EndPopup();
        }
        if (ImGui::Button("Options")) {
            ImGui::OpenPopup("Options");
        }

        ImGui::SameLine();

        // Command input
        bool reclaim_focus = false;
        ImGuiInputTextFlags input_text_flags = ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CallbackCompletion | ImGuiInputTextFlags_CallbackHistory;
        const auto on_text_callback = [](ImGuiInputTextCallbackData* data) {
            auto this_console = static_cast<imgui_console_command*>(data->UserData);

            switch (data->EventFlag) {
            case ImGuiInputTextFlags_CallbackCompletion: {
                const auto candidates = command_parser::autocomplete(data->Buf);
                log(LoggerName::EDITOR, "Autocompletion found {} candidates for {}:\n", candidates.size(), data->Buf);
                if (candidates.size() == 1) {
                    data->DeleteChars(0, data->BufTextLen);
                    data->InsertChars(0, candidates[0].data());
                }
                for (const auto& command : candidates) {
                    log(LoggerName::EDITOR, "    -{}\n", command);
                }
                break;
            }
            case ImGuiInputTextFlags_CallbackHistory: {
                const int prev_command_index = this_console->my_command_history.current_command_index();
                int current_command_index = prev_command_index;
                if (data->EventKey == ImGuiKey_UpArrow) {
                    if (current_command_index == -1) {
                        current_command_index = static_cast<int>(this_console->my_command_history.size()) - 1;
                    }
                    else if (current_command_index > 0) {
                        current_command_index--;
                    }
                }
                else if (data->EventKey == ImGuiKey_DownArrow) {
                    if (current_command_index != -1 && this_console->my_command_history.at(current_command_index) != "") {
                        if (++current_command_index >= static_cast<int>(this_console->my_command_history.size())) {
                            current_command_index = -1;
                        }
                    }
                }

                if (prev_command_index != current_command_index)
                {
                    this_console->my_command_history.set_current_command_index(current_command_index);

                    const char* history_str = (current_command_index >= 0) ? this_console->my_command_history.at(current_command_index).data() : "";
                    data->DeleteChars(0, data->BufTextLen);
                    data->InsertChars(0, history_str);
                }
            }
            }
            return 0;
        };
        if (ImGui::InputText("Input", my_command_buffer.data(), my_command_buffer.size(), input_text_flags, on_text_callback, (void*)this)) {
            my_command_history.add(command_parser::parse(my_command_buffer.data()));
            my_command_buffer[0] = 0;
            reclaim_focus = true;
        }

        // Auto-focus on window apparition
        ImGui::SetItemDefaultFocus();
        if (reclaim_focus) {
            ImGui::SetKeyboardFocusHere(-1); // Auto focus previous widget
        }

        // Log command history widget
        ImGui::BeginChild("ScrollingRegion", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);
        if (ImGui::BeginPopupContextWindow()) {
            if (ImGui::Selectable("Clear")) {
                my_command_history.clear();
            }
            ImGui::EndPopup();
        }

        for (const auto& command : my_command_history) {
            ImVec4 color;
            bool has_color = false;
            if (command.find("[error]") != string::npos) {
                color = ImVec4(1.0f, 0.4f, 0.4f, 1.0f);
                has_color = true;
            }
            if (has_color) {
                ImGui::PushStyleColor(ImGuiCol_Text, color);
            }
            ImGui::TextUnformatted(command.c_str());
            if (has_color) {
                ImGui::PopStyleColor();
            }
        }

        if (enable_auto_scroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY()) {
            ImGui::SetScrollHereY(1.0f);
        }

        ImGui::EndChild();
    }
    ImGui::End();

}