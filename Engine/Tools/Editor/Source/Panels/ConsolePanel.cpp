module;
#include "cspch.hpp"
#include <imgui.h>
#include <algorithm>
module Editor.Panels.ConsolePanel;

CS_MODULE_LOG_INFO(Editor, Panels.ConsolePanel);

namespace Cosmic
{

    static ImVec4 ELogSeverityToImGuiColor(ELogSeverity severity)
    {
        switch (severity)
        {
            case ELogSeverity::Trace:    return ImVec4(1.0f,   1.0f,    1.0f,   1.0f);
            case ELogSeverity::Info:     return ImVec4(0.231f, 0.827f,  0.322f, 1.0f);
            case ELogSeverity::Debug:    return ImVec4(0.196f, 0.278f,  0.812f, 1.0f);
            case ELogSeverity::Warn:     return ImVec4(0.851f, 1.0f,    0.263f, 1.0f);
            case ELogSeverity::Error:    return ImVec4(1.0f,   0.184f,  0.184f, 1.0f);
            case ELogSeverity::Critical: return ImVec4(1.0f,   0.0f,    0.0f,   1.0f);
        }
    }

    static const char* ELogSeverityToStr(ELogSeverity severity)
    {
        if (severity == ELogSeverity::Trace)         return "Trace";
        else if (severity == ELogSeverity::Info)     return "Info";
        else if (severity == ELogSeverity::Debug)    return "Debug";
        else if (severity == ELogSeverity::Warn)     return "Warn";
        else if (severity == ELogSeverity::Error)    return "Error";
        else if (severity == ELogSeverity::Critical) return "Critical";
        return "";
    }

    ConsolePanel::ConsolePanel()
        : Panel("Console Panel")
    {
    }

    void ConsolePanel::OnEvent(const Event& e)
    {
        CS_PROFILE_FN();

        EventDispatcher dispatcher(e);
        CS_DISPATCH_EVENT(LogEvent, OnLog);
    }

    bool ConsolePanel::OnLog(const LogEvent& e)
    {
        CS_PROFILE_FN();

        mLogQueue.push_back(e);

        return false;
    }

    void ConsolePanel::OnImGuiRender()
    {
        CS_PROFILE_FN();

        if (ImGui::Begin("Console", &mOpen))
        {
            static bool showTraceLogs    = true;
            static bool showInfoLogs     = true;
            static bool showDebugLogs    = true;
            static bool showWarnLogs     = true;
            static bool showErrorLogs    = true;
            static bool showCriticalLogs = true;

            static bool showFiles = true;
            static bool showSeverity = true;
            static bool showMessage = true;

            uint32 width = ImGui::GetContentRegionAvail().x;
            uint32 height = ImGui::GetContentRegionAvail().y;

            uint32 checkboxDivisions = width / 6;

            static ImGuiTextFilter filter;

            if (ImGui::CollapsingHeader("Properties"))
            {
                ImGui::Checkbox("Trace", &showTraceLogs);
                ImGui::SameLine(checkboxDivisions * 1);
                ImGui::Checkbox("Info", &showInfoLogs);
                ImGui::SameLine(checkboxDivisions * 2);
                ImGui::Checkbox("Debug", &showDebugLogs);
                ImGui::SameLine(checkboxDivisions * 3);
                ImGui::Checkbox("Warn", &showWarnLogs);
                ImGui::SameLine(checkboxDivisions * 4);
                ImGui::Checkbox("Error", &showErrorLogs);
                ImGui::SameLine(checkboxDivisions * 5);
                ImGui::Checkbox("Critical", &showCriticalLogs);

                ImGui::Checkbox("Show Files", &showFiles);
                ImGui::SameLine(checkboxDivisions * 1);
                ImGui::Checkbox("Show Severity", &showSeverity);
                ImGui::SameLine(checkboxDivisions * 2);
                ImGui::Checkbox("Show Message", &showMessage);

                filter.Draw();

                ImGui::SameLine(checkboxDivisions * 5);

                if (ImGui::Button("Clear"))
                    mLogQueue.clear();
            }

            ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(5.0f, 5.0f));
            ImGui::BeginChild("Logs", ImVec2((float32)width, ImGui::GetContentRegionAvail().y), true);

            uint32 columnCount = (uint32)showFiles + (uint32)showSeverity + (uint32)showMessage;
            float32 textWidth = ImGui::CalcTextSize("A").x;

            if (columnCount != 0)
            {
                if (ImGui::BeginTable("Logs Table", columnCount, ImGuiTableFlags_Resizable | ImGuiTableFlags_NoSavedSettings | ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_ScrollY))
                {
                    if (showFiles)    ImGui::TableSetupColumn("Files");
                    if (showSeverity) ImGui::TableSetupColumn("Severity");
                    if (showMessage)  ImGui::TableSetupColumn("Message");

                    ImGui::TableHeadersRow();

                    for (LogEvent e : mLogQueue)
                    {
                        if (!filter.PassFilter(e.GetMessage().c_str()))
                            continue;

                        ELogSeverity severity    = e.GetSeverity();
                        ImVec4       color       = ELogSeverityToImGuiColor(e.GetSeverity());
                        String       msgWithFile = e.GetMessage();
                        StringView   file        = StringView(msgWithFile.c_str(), msgWithFile.find_first_of(':') + 1);
                        StringView   msg         = StringView(msgWithFile.c_str() + msgWithFile.find_first_of(':') + 2, msgWithFile.size());

                        char* data = (char*)file.data();
                        data[file.size() - 1] = 0;

                        if (!showTraceLogs    && severity == ELogSeverity::Trace)    continue;
                        if (!showInfoLogs     && severity == ELogSeverity::Info)     continue;
                        if (!showDebugLogs    && severity == ELogSeverity::Debug)    continue;
                        if (!showWarnLogs     && severity == ELogSeverity::Warn)     continue;
                        if (!showErrorLogs    && severity == ELogSeverity::Error)    continue;
                        if (!showCriticalLogs && severity == ELogSeverity::Critical) continue;

                        if (showFiles)
                        {
                            ImGui::TableNextColumn();
                            ImGui::Text(file.data());
                        }
                        if (showSeverity)
                        {
                            ImGui::TableNextColumn();
                            ImGui::TextColored(color, ELogSeverityToStr(e.GetSeverity()));
                        }
                        if (showMessage)
                        {
                            ImGui::TableNextColumn();
                            ImGui::TextColored(color, msg.data());
                        }
                    }
                    ImGui::EndTable();
                }
            }

            ImGui::PopStyleVar(2);
            ImGui::EndChild();
        }
        ImGui::End();
    }

}