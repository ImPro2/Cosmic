#include "App/FileSystem.hpp"
#include "App/Log/LogMacros.hpp"
#include "Base/Macros.hpp"
#include "cspch.hpp"
#include "FileDialog.hpp"
#include "Time/Time.hpp"
#include "Project/ProjectManager.hpp"

#include <string.h>
#include <filesystem>
#include <imgui.h>
#include <imgui_internal.h>
#include <IconsFontAwesome6.h>

CS_MODULE_LOG_INFO(Editor, UI.FileDialog);

namespace Cosmic
{

    namespace Utils
    {

        static void SortAlphabetically(Vector<Path>& strings)
        {
            std::sort(strings.begin(), strings.end(), [](const Path& first, const Path& second)
            {
                const String& s1 = first.GetString();
                const String& s2 = second.GetString();

                if (s1[0] == '.' && s2[0] != '.')
                    return true;
                else if (s1[0] != '.' && s2[0] == '.')
                    return false;

                return s1 > s2;
            });
        }

    }

    FileDialogModule::FileDialogModule(const Path& dir)
        : mDirectory(dir)
    {
        if (mDirectory.GetString().empty())
        {
            if (Ref<Project> project = ProjectManager::GetActiveProject())
                mDirectory = project->GetParentPath();
            else
                mDirectory = FileSystem::GetCurrentWorkingDirectory();
        }

        memset(mFileInputBuffer, 0, 256);
        memset(mSearchInputBuffer, 0, 256);
        memset(mDirectoryInputBuffer, 0, 256);

        mSelectedItemIndex = -1;

        mSelectedPath = String("");
    }

    void FileDialogModule::OnInit()
    {
    }

    void FileDialogModule::OnImGuiRender()
    {
        ImGui::OpenPopup(mWindowName.c_str());

        ImVec2 size = ImVec2(ImGui::GetMainViewport()->Size.x * 0.5f, ImGui::GetMainViewport()->Size.y * 0.5f);
        ImVec2 center = ImGui::GetMainViewport()->GetCenter();
        ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
        ImGui::SetNextWindowSize(size, ImGuiCond_Always);
        
        ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize;
        
        if (ImGui::BeginPopupModal(mWindowName.c_str(), nullptr, windowFlags))
        {
            RenderTop();
            RenderDirectoryTree();

            switch (mDialogMode)
            {
                case EDialogMode::Open: RenderOpenFileWidgets(); break;
                case EDialogMode::Save: RenderSaveFileWidgets(); break;
            }

            ImGui::EndPopup();
        }

        if (mClosedDialog)
            CloseDialog();
    }
    
    void FileDialogModule::RenderTop()
    {
        if (ImGui::Button(ICON_FA_ARROW_UP))
            SetDirectory(FileSystem::GetParentDirectory(mDirectory));

        ImGui::SameLine();
        if (ImGui::InputText("|", mDirectoryInputBuffer, 256, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll))
        {
            if (FileSystem::Exists(mDirectoryInputBuffer)) 
                SetDirectory(String(mDirectoryInputBuffer));
            else
                strcpy(mDirectoryInputBuffer, mDirectory.GetString().c_str());
        }

        ImGui::SameLine();
        ImGui::InputTextWithHint(ICON_FA_MAGNIFYING_GLASS, "Search...", mSearchInputBuffer, 256, ImGuiInputTextFlags_AutoSelectAll);
    }

    void FileDialogModule::RenderDirectoryTree()
    {
        float32 width  = (float32)ImGui::GetContentRegionAvail().x;
        float32 height = (float32)ImGui::GetContentRegionAvail().y;

        ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(5.0f, 5.0f));

        float32 tableWidth = width - 10.0f;
        float32 tableHeight = height;
        
        switch (mDialogMode)
        {
            case EDialogMode::Open: tableHeight = height * 0.8f; break; 
            case EDialogMode::Save: tableHeight = height * 0.7f; break;
        }

        ImGui::BeginChild("FileTree", ImVec2(width, tableHeight), true);

        float32 col1Width = tableWidth * 0.7f;
        float32 col2Width = tableWidth * 0.15f;
        float32 col3Width = tableWidth * 0.15f;

        uint32 columnCount = 3;
        ImGuiTableFlags tableFlags = ImGuiTableFlags_Resizable | ImGuiTableFlags_NoSavedSettings | ImGuiTableFlags_ScrollY | ImGuiTableFlags_RowBg;
        
        if (ImGui::BeginTable("Directory Table", columnCount, tableFlags))
        {
            ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthStretch, col1Width);
            ImGui::TableSetupColumn("Size", ImGuiTableColumnFlags_WidthStretch, col2Width);
            ImGui::TableSetupColumn("Type", ImGuiTableColumnFlags_WidthStretch, col3Width);

            ImGui::TableHeadersRow();

            for (int32 i = 0; i < mDirectoryContents.size(); i++)
            { 
                const Path& path = mDirectoryContents[i];

                if (path.GetBase().find(mSearchInputBuffer))
                    continue;

                bool isFile = FileSystem::IsFile(path);

                if (mDialogMode == EDialogMode::Open && isFile)
                {
                    File file = File(path);    

                    if (!mOpenFileFilters.empty())
                    {
                        bool isFileIncluded = false;
                        for (const String& filter : mOpenFileFilters)
                        {
                            String ext = file.GetExtension();
                            if (String(ext) == filter)
                            {
                                isFileIncluded = true;
                                break;
                            }
                        }
                        
                        if (!isFileIncluded)
                            continue;
                    }
                }

                ImGui::TableNextColumn();
                if (ImGui::Selectable(path.GetBase().c_str(), mSelectedItemIndex == i, ImGuiSelectableFlags_SpanAllColumns))
                {
                    mSelectedItemIndex = i;
                    if (mDialogMode == EDialogMode::Open && isFile)
                    {
                        strncpy(mFileInputBuffer, path.GetBase().c_str(), 256);
                        mSelectedPath = path;
                    }
                }

                if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenOverlapped) && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
                {
                    if (isFile && mDialogMode == EDialogMode::Open)
                    {
                        mSelectedPath = path;
                        mClosedDialog = true;
                        ImGui::CloseCurrentPopup();
                    }
                    else
                        SetDirectory(path);

                    break;
                }

                if (isFile)
                {
                    File file = File(path);    

                    ImGui::TableNextColumn();
                    ImGui::Text("%i bytes", (int32)file.GetSize());

                    ImGui::TableNextColumn();
                    ImGui::Text("file");
                }
                else
                {
                    int32 items = FileSystem::ListDirectoryContents(path).size();

                    ImGui::TableNextColumn();
                    ImGui::Text("%i items", items);

                    ImGui::TableNextColumn();
                    ImGui::Text("directory");
                }
            }

            ImGui::EndTable();
        }

        ImGui::PopStyleVar(2);
        ImGui::EndChild();
    }

    void FileDialogModule::RenderOpenFileWidgets()
    {
        ImGui::Text("Selected File: ");
        ImGui::SameLine();
        ImGui::InputText("", mFileInputBuffer, 256, ImGuiInputTextFlags_AutoSelectAll);
        ImGui::SameLine();
        ImGui::Text("Filters: ");
        ImGui::SameLine();

        if (ImGui::BeginCombo("Filters", mOpenFileFilters[0].c_str()))
        {
            for (int32 i = 0; i < mOpenFileFilters.size(); i++)
            {
                ImGui::SetNextItemWidth(100);
                ImGui::Selectable(mOpenFileFilters[i].c_str(), false);
            }

            ImGui::EndCombo();
        }

        ImGui::Text("");
        ImGui::SameLine(ImGui::GetContentRegionAvail().x - 100.0f);

        ImGui::CalcItemWidth();
        
        if (ImGui::Button("Cancel"))
            CloseDialog(true);

        ImGui::SameLine();
        
        if (ImGui::Button("Open"))
            mClosedDialog = true;
    }

    void FileDialogModule::RenderSaveFileWidgets()
    {
        ImGui::Text("File name: ");
        ImGui::SameLine();

        ImGui::PushID(10);
        ImGui::InputText("", mFileInputBuffer, 256);
        ImGui::PopID();

        ImGui::Text("Save type: ");
        ImGui::SameLine();

        ImGui::PushID(12);
        if (ImGui::BeginCombo("", mSaveFiletypes[0].c_str()))
        {
            for (const String& saveFileType : mSaveFiletypes)
                ImGui::Selectable(saveFileType.c_str());

            ImGui::EndCombo();
        }
        ImGui::PopID();

        ImGui::Text("");
        ImGui::SameLine(ImGui::GetContentRegionAvail().x - 100.0f);

        ImGui::CalcItemWidth();
        
        if (ImGui::Button("Cancel"))
            CloseDialog(true);

        ImGui::SameLine();
        
        if (ImGui::Button("Save"))
        {
            mClosedDialog = true;
            mSelectedPath = mDirectory / String(mFileInputBuffer);
        }

    }

    void FileDialogModule::SetOpenFileCallback(const String& description, const Vector<String>& filters, FileDialogCallback callback)
    {
        mOpenFileDescription = description;
        mCallback = callback;
        mWindowName = "Open File...";
        mOpenFileFilters = filters;
        mDialogMode = EDialogMode::Open;

        SetDirectory(mDirectory);
        strcpy(mDirectoryInputBuffer, mDirectory.GetString().c_str());
    }

    void FileDialogModule::SetSaveFileCallback(const String& defaultSaveFilename, const Vector<String>& filetypes, FileDialogCallback callback)
    {
        mCallback = callback;
        mSaveFiletypes = filetypes;
        mDefaultSaveFilename = defaultSaveFilename;
        mWindowName = "Save File...";
        mDialogMode = EDialogMode::Save;
        mSelectedPath = mDirectory;

        SetDirectory(mDirectory);
        strcpy(mDirectoryInputBuffer, mDirectory.GetString().c_str());
        strcpy(mFileInputBuffer, mDefaultSaveFilename.c_str());
    }

    void FileDialogModule::SetDirectory(const Path& newDir)
    {
        mDirectory = newDir;
        mDirectoryContents = FileSystem::ListDirectoryContents(mDirectory);
        Utils::SortAlphabetically(mDirectoryContents);

        strcpy(mDirectoryInputBuffer, mDirectory.GetString().c_str());
    }

    void FileDialogModule::CloseDialog(bool cancelled)
    {
        if (!mSelectedPath.GetString().empty() && !cancelled)
        {
            mCallback(File(mSelectedPath));
        }

        ModuleSystem::RemoveDeferred<FileDialogModule>();
    }

}
