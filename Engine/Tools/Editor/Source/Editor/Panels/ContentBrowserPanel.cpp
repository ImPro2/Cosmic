#include "cspch.hpp"
#include "ContentBrowserPanel.hpp"
#include "App/Event/Events.hpp"
#include "Project/ProjectManager.hpp"
#include "App/FileSystem.hpp"

#include <imgui.h>
#include <IconsFontAwesome6.h>

namespace Cosmic
{

    ContentBrowserPanel::ContentBrowserPanel()
        : Panel("Content Browser")
    {
    }
    
    void ContentBrowserPanel::OnInit()
    {
        // TODO: When projects are fully implemented
        //const ProjectInfo& projectInfo = ProjectManager::GetActiveProject()->GetInfo();
        //mProjectDirectory = FileSystem::GetParentDirectory(projectInfo.ProjectFilePath.GetAbsolutePath());
        
        mProjectDirectory = "Engine/Tools/Editor/";
        
        mCurrentDirectory = mProjectDirectory;
        UpdateContents();
    }

    void ContentBrowserPanel::OnEvent(const Event& e)
    {
        EventDispatcher dispatcher(e);

        CS_DISPATCH_EVENT(FileAddedEvent, OnFileAdded);
        CS_DISPATCH_EVENT(FileRemovedEvent, OnFileRemoved);
        CS_DISPATCH_EVENT(FileRenamedEvent, OnFileRenamed);
        CS_DISPATCH_EVENT(FileModifiedEvent, OnFileModified);
        CS_DISPATCH_EVENT(DirectoryAddedEvent, OnDirectoryAdded);
        CS_DISPATCH_EVENT(DirectoryRemovedEvent, OnDirectoryRemoved);
        CS_DISPATCH_EVENT(DirectoryRenamedEvent, OnDirectoryRenamed);
        CS_DISPATCH_EVENT(DirectoryModifiedEvent, OnDirectoryModified);
    }

    void ContentBrowserPanel::OnImGuiRender()
    {
        if (!mOpen)
            return;

        if (ImGui::Begin(mPanelName.c_str()))
        {
            RenderTop();
            RenderContents();
        }

        ImGui::End();
    }

    void ContentBrowserPanel::UpdateContents()
    {
        mDirectoryContents = FileSystem::ListDirectoryContents(mCurrentDirectory);
    }
    
    void ContentBrowserPanel::RenderTop()
    {

    }

    void ContentBrowserPanel::RenderContents()
    {
        ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(5.0f, 5.0f));
        ImGui::BeginChild("Contents Child", ImVec2(), true);

        float32 childWidth = ImGui::GetItemRectMax().x;

        for (Path path : mDirectoryContents)
        {
            RenderContentItem(path);

            float32 contentRegionAvail = childWidth - ImGui::GetItemRectMax().x - mContentItemSize - mContentItemSize * 0.5f;

            if (contentRegionAvail > 0.0f)
            {
                ImGui::GetCursorPos();
                ImGui::SameLine();
            }
        }

        ImGui::EndChild();
        ImGui::PopStyleVar(2);
    }
    
    void ContentBrowserPanel::RenderContentItem(const Path& path)
    {
        const char* icon = "";

        if (FileSystem::IsFile(path))
        {
            icon = ICON_FA_FILE;
        }
        else if (FileSystem::IsDirectory(path))
        {
            icon = ICON_FA_FOLDER_OPEN;
        }

        float32 itemSpacing = ImGui::GetStyle().ItemSpacing.y;
        
        ImVec2 padding = ImVec2(5.0f, 5.0f);

        // this line is _absolutely perfect_. This single line took half an hour
        ImVec2 childSize = ImVec2(mContentItemSize + 2.0f * padding.x, mContentItemSize + ImGui::GetTextLineHeight() + 2.0f * padding.y + itemSpacing);
        
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, padding);
        ImGui::BeginChild(path.GetString().c_str(), childSize, true);

        ImGui::PushID(path.GetBase().c_str());
        ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[1]);
        bool open = ImGui::Button(icon, ImVec2(mContentItemSize, mContentItemSize));
        ImGui::PopFont();
        ImGui::PopID();

        const char* base = path.GetBase().c_str();
        float32 baseWidth = ImGui::CalcTextSize(base).x;
        ImGui::SetCursorPosX((childSize.x - baseWidth) * 0.5f); // center text
        ImGui::Text("%s", base);
        
        ImGui::EndChild();
        ImGui::PopStyleVar();
    }

    bool ContentBrowserPanel::OnFileAdded(const FileAddedEvent& e)
    {
        UpdateContents();
        return false;
    }

    bool ContentBrowserPanel::OnFileRemoved(const FileRemovedEvent& e)
    {
        UpdateContents();
        return false;
    }

    bool ContentBrowserPanel::OnFileRenamed(const FileRenamedEvent& e)
    {
        UpdateContents();
        return false;
    }

    bool ContentBrowserPanel::OnFileModified(const FileModifiedEvent& e)
    {
        return false;
    }

    bool ContentBrowserPanel::OnDirectoryAdded(const DirectoryAddedEvent& e)
    {
        UpdateContents();
        return false;
    }

    bool ContentBrowserPanel::OnDirectoryRemoved(const DirectoryRemovedEvent& e)
    {
        UpdateContents();
        return false;
    }

    bool ContentBrowserPanel::OnDirectoryRenamed(const DirectoryRenamedEvent& e)
    {
        UpdateContents();
        return false;
    }

    bool ContentBrowserPanel::OnDirectoryModified(const DirectoryModifiedEvent& e)
    {
        return false;
    }

}
