#include "cspch.hpp"
#include "App/KeyAndMouseCodes.hpp"
#include "cspch.hpp"
#include "ContentBrowserPanel.hpp"
#include "Project/ProjectManager.hpp"
#include "App/FileSystem.hpp"
#include "App/Input.hpp"
#include "UI/ImGuiUtil.hpp"
#include "imgui.h"

#include <IconsFontAwesome6.h>

namespace Cosmic
{

    ContentBrowserPanel::ContentBrowserPanel()
        : IPanel("Content Browser")
    {
    }
    
    void ContentBrowserPanel::OnInit()
    {
        // TODO: When projects are fully implemented
        //const ProjectInfo& projectInfo = ProjectManager::GetActiveProject()->GetInfo();
        //mProjectDirectory = FileSystem::GetParentDirectory(projectInfo.ProjectFilePath.GetAbsolutePath());
        
        mProjectDirectory = "Engine/Tools/Editor";
        
        mCurrentDirectory = mProjectDirectory;
        UpdateContents();
    }

    void ContentBrowserPanel::OnEvent(const IEvent& e)
    {
        EventDispatcher dispatcher(e);

        CS_DISPATCH_EVENT(KeyPressEvent, OnKeyPressed);
        CS_DISPATCH_EVENT(MouseScrollEvent, OnMouseScrolled);
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

        mWindowHovered = ImGui::IsWindowHovered(ImGuiHoveredFlags_RootAndChildWindows);

        ImGui::End();
    }

    void ContentBrowserPanel::UpdateContents()
    {
        mDirectoryContents = FileSystem::ListDirectoryContents(mCurrentDirectory);
        mContentItemHoveredIndex = -1;
        mSelectedContentItemIndices.clear();

        memset(mDirectoryInputBuffer, 0, 256);
        strcpy(mDirectoryInputBuffer, mCurrentDirectory.GetString().c_str());
    }
    
    void ContentBrowserPanel::RenderTop()
    {
        ImGui::PushID("Content Browser Panel Goto Parent Directory Button");
        bool gotoParentDir = ImGui::Button(ICON_FA_ARROW_UP);
        ImGui::PopID();

        if (gotoParentDir)
        {
            mCurrentDirectory = FileSystem::GetParentDirectory(mCurrentDirectory);
            UpdateContents();
        }

        ImGui::SameLine();

        ImGuiInputTextFlags inputTextFlags = ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue;

        ImGui::PushID("Content Browser Panel Directory Input Text");
        bool entered = ImGui::InputText("", mDirectoryInputBuffer, 256, inputTextFlags);
        ImGui::PopID();

        if (entered)
        {
            Path inputPath = Path(mDirectoryInputBuffer);

            if (FileSystem::Exists(inputPath))
            {
                mCurrentDirectory = inputPath;
                UpdateContents();
            }
            else
                strcpy(mDirectoryInputBuffer, mCurrentDirectory.GetString().c_str());
        }
    }

    void ContentBrowserPanel::RenderContents()
    {
        ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(5.0f, 5.0f));
        ImGui::BeginChild("Contents Child", ImVec2(), true);

        float32 childWidth = ImGui::GetItemRectMax().x;

        for (int32 i = 0; i < mDirectoryContents.size(); i++)
        {
            const Path& path = mDirectoryContents[i];
            bool contentsChanged = RenderContentItem(path, i);

            if (contentsChanged)
                break;

            float32 contentRegionAvail = childWidth - ImGui::GetItemRectMax().x - mContentItemSize - mContentItemSize * 0.5f;

            if (contentRegionAvail > 0.0f)
            {
                ImGui::GetCursorPos();
                ImGui::SameLine();
            }
        }

        RenderRightClickMenu();

        //if (ImGui::IsWindowHovered() || mMouseSelectionStarted)
        //    mMouseSelectionStarted = !ImGuiUtil::SelectionRect(&mMouseSelectionStart, &mMouseSelectionEnd, ImGuiMouseButton_Left);

        ImGui::EndChild();
        ImGui::PopStyleVar(2);
    }
    
    bool ContentBrowserPanel::RenderContentItem(const Path& path, int32 index)
    {
        const char* icon = FileSystem::IsFile(path) ? ICON_FA_FILE : ICON_FA_FOLDER_OPEN;

        float32 itemSpacing = ImGui::GetStyle().ItemSpacing.y;
        
        ImVec2 padding = ImVec2(5.0f, 5.0f);
        // this line is _absolutely perfect_. This single line took half an hour
        ImVec2 childSize = ImVec2(mContentItemSize + 2.0f * padding.x, mContentItemSize + ImGui::GetTextLineHeight() + 2.0f * padding.y + itemSpacing);
        
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, padding);

        bool isSelected = std::find(mSelectedContentItemIndices.begin(), mSelectedContentItemIndices.end(), index) != mSelectedContentItemIndices.end();

        if (index == mContentItemHoveredIndex)
        {
            ImGui::PushStyleColor(ImGuiCol_ChildBg, ImGui::GetStyleColorVec4(ImGuiCol_TabActive));
            mContentItemHoveredIndex = -1;
        }
        else if (isSelected)
            ImGui::PushStyleColor(ImGuiCol_ChildBg, ImGui::GetStyleColorVec4(ImGuiCol_HeaderActive));
        else
            ImGui::PushStyleColor(ImGuiCol_ChildBg, ImGui::GetStyleColorVec4(ImGuiCol_TableRowBgAlt));
        
        ImGui::BeginChild(path.GetString().c_str(), childSize, true);

        // Selection & clicking
        
        bool contentsChanged = false;

        if (mMouseSelectionStarted && std::find(mSelectedContentItemIndices.begin(), mSelectedContentItemIndices.end(), index) == mSelectedContentItemIndices.end() && !ImGui::IsMouseReleased(ImGuiMouseButton_Left) && !ImGui::IsMouseClicked(ImGuiMouseButton_Left) && !Input::IsKeyPressed(EKeyCode::LeftControl))
        {
            ImVec2 topLeft  = ImGui::GetWindowPos();
            ImVec2 btmRight = ImVec2(topLeft.x + childSize.x, topLeft.y + childSize.y);

            ImVec2 mouseTopLeft  = ImVec2(std::min(mMouseSelectionStart.x, mMouseSelectionEnd.x), std::min(mMouseSelectionStart.y, mMouseSelectionEnd.y));
            ImVec2 mouseBtmRight = ImVec2(std::max(mMouseSelectionStart.x, mMouseSelectionEnd.x), std::max(mMouseSelectionStart.y, mMouseSelectionEnd.y));

            if (((mouseTopLeft.x  > topLeft.x && mouseTopLeft.y  > topLeft.y) && (mouseTopLeft.x  < btmRight.x && mouseTopLeft.y  < btmRight.y)) ||
                ((mouseBtmRight.x > topLeft.x && mouseBtmRight.y > topLeft.y) && (mouseBtmRight.x < btmRight.x && mouseBtmRight.y < btmRight.y)))
            {
                mSelectedContentItemIndices.push_back(index);
            }
        }

        if (ImGui::IsWindowHovered(ImGuiHoveredFlags_ChildWindows))
        {
            mContentItemHoveredIndex = index;

            if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left) && FileSystem::IsDirectory(path))
            {
                mCurrentDirectory /= path.GetBase();
                UpdateContents();
                contentsChanged = true;
            }
            else if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
            {
                bool control = Input::IsKeyPressed(EKeyCode::LeftControl) || Input::IsKeyPressed(EKeyCode::RightControl);
                bool shift   = Input::IsKeyPressed(EKeyCode::LeftShift)   || Input::IsKeyPressed(EKeyCode::RightShift);

                if (control)
                {
                    auto it = std::find(mSelectedContentItemIndices.begin(), mSelectedContentItemIndices.end(), index);
                    
                    if (it == mSelectedContentItemIndices.end())
                        mSelectedContentItemIndices.push_back(index);
                    else
                        mSelectedContentItemIndices.erase(it);
                }
                else if (shift)
                {
                    if (mSelectedContentItemIndices.empty())
                        mSelectedContentItemIndices.push_back(index);
                    else
                    {
                        int32 lastSelelectedIdx = mSelectedContentItemIndices[mSelectedContentItemIndices.size() - 1];
                        
                        if (index > lastSelelectedIdx)
                        {
                            for (int32 i = lastSelelectedIdx + 1; i <= index; i++)
                                mSelectedContentItemIndices.push_back(i);
                        }
                        else if (index < lastSelelectedIdx)
                        {
                            for (int32 i = lastSelelectedIdx - 1; i >= index; i--)
                                mSelectedContentItemIndices.push_back(i);
                        }
                    }
                }
                else
                {
                    mSelectedContentItemIndices.clear();
                    mSelectedContentItemIndices.push_back(index);
                }
            }
        }

        if (!contentsChanged)
        {
            // Thumbnail

            ImGui::PushID(path.GetBase().c_str());
            ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[1]);

            bool open = ImGui::Button(icon, ImVec2(mContentItemSize, mContentItemSize));

            // Drag&Drop

            if (FileSystem::IsFile(path))
            {
                File file(path);

                if (file.GetExtension() == ".cscene" && ImGui::BeginDragDropSource())
                {
                    ImGui::SetDragDropPayload(mContentItemDragDropString.c_str(), path.GetString().c_str(), path.GetString().size());
                    ImGui::EndDragDropSource();
                }
            }

            ImGui::PopFont();
            ImGui::PopID();

            // Filename

            char* base = (char*)alloca(path.GetBase().size() + 1);
            strcpy(base, path.GetBase().c_str());
            base[path.GetBase().size()] = '\0';

            float32 baseWidth = ImGui::CalcTextSize(base).x;

            if (baseWidth < childSize.x)
            {
                ImGui::SetCursorPosX((childSize.x - baseWidth) * 0.5f); // center text
                ImGui::Text("%s", base);
            }
            else
            {
                float32 avgGlyphWidth = baseWidth / path.GetBase().size();
                int32 maxCharCount = childSize.x / avgGlyphWidth;
                maxCharCount -= 3; // for the ...

                base[maxCharCount - 1] = '\0';

                ImGui::Text("%s...", base);
            }
        }

        ImGui::PopStyleColor();
        
        ImGui::EndChild();
        ImGui::PopStyleVar();

        return contentsChanged;
    }

    void ContentBrowserPanel::RenderRightClickMenu()
    {
        if (ImGui::IsMouseReleased(ImGuiMouseButton_Right) && ImGui::IsWindowHovered(ImGuiHoveredFlags_RootAndChildWindows))
            ImGui::OpenPopup(sPopupID);

        if (ImGui::BeginPopup(sPopupID))
        {
            if (ImGui::MenuItem("Select All", "CTRL+A"))
            {
                SelectAll();
                ImGui::CloseCurrentPopup();
            }

            if (ImGui::MenuItem("Delete", "DEL", false, !mSelectedContentItemIndices.empty()))
            {
                DeleteSelected();
                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }
    }

    void ContentBrowserPanel::SelectAll()
    {
        mSelectedContentItemIndices.clear();
        mSelectedContentItemIndices.resize(mDirectoryContents.size());

        for (int32 i = 0; i < mDirectoryContents.size(); i++)
        {
            mSelectedContentItemIndices[i] = i;
        }
    }

    void ContentBrowserPanel::DeleteSelected()
    {

    }

    bool ContentBrowserPanel::OnKeyPressed(const KeyPressEvent& e)
    {
        if (!mWindowHovered)
            return false;
        
        bool control = Input::IsKeyPressed(EKeyCode::LeftControl) || Input::IsKeyPressed(EKeyCode::LeftControl);

        switch (e.GetKeyCode())
        {
            case EKeyCode::A:
            {
                if (control)
                    SelectAll();
                break;
            }
            case EKeyCode::Delete:
            {
                DeleteSelected();
                break;
            }
        }

        return false;
    }

    bool ContentBrowserPanel::OnMouseScrolled(const MouseScrollEvent& e)
    {
        if (Input::IsKeyPressed(EKeyCode::LeftControl) || Input::IsKeyPressed(EKeyCode::RightControl))
            mContentItemSize += e.GetOffset();

        return false;
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

    Vector<Path> ContentBrowserPanel::GetSelectedContentItems()
    {
        Vector<Path> selectedContentItems(mSelectedContentItemIndices.size());

        for (int32 i = 0; i < mSelectedContentItemIndices.size(); i++)
        {
            selectedContentItems.push_back(mDirectoryContents[mSelectedContentItemIndices[i]]);
        }

        return selectedContentItems;
    }

}
