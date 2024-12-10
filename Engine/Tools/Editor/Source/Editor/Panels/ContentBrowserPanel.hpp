#pragma once
#include "Panels.hpp"

#include "App/Path.hpp"
#include "Event/Type/WindowEvents.hpp"
#include "Event/Type/FileSystemEvents.hpp"

#include <imgui.h>

namespace Cosmic
{

    class ContentBrowserPanel : public IPanel
    {
    public:
        ContentBrowserPanel();

    public:
        void OnInit()                 override;
        void OnEvent(const IEvent& e) override;
        void OnImGuiRender()          override;

        Vector<Path> GetSelectedContentItems();
        const String& GetContentItemDragDropString() { return mContentItemDragDropString; }

    private:
        void UpdateContents();

    private:
        void RenderTop();
        void RenderContents();
        bool RenderContentItem(const Path& path, int32 index);
        void RenderRightClickMenu();

    private:
        void SelectAll();
        void DeleteSelected();

    private:
        bool OnKeyPressed(const KeyPressEvent& e);
        bool OnMouseScrolled(const MouseScrollEvent& e);

        bool OnFileAdded(const FileAddedEvent& e);
        bool OnFileRemoved(const FileRemovedEvent& e);
        bool OnFileRenamed(const FileRenamedEvent& e);
        bool OnFileModified(const FileModifiedEvent& e);

        bool OnDirectoryAdded(const DirectoryAddedEvent& e);
        bool OnDirectoryRemoved(const DirectoryRemovedEvent& e);
        bool OnDirectoryRenamed(const DirectoryRenamedEvent& e);
        bool OnDirectoryModified(const DirectoryModifiedEvent& e);

    private:
        Path mProjectDirectory;
        Path mCurrentDirectory;

        //int32 mContentTableColumnCount;

        Vector<Path> mDirectoryContents;
        Vector<int32> mSelectedContentItemIndices;
        int32 mContentItemHoveredIndex;

        float32 mContentItemSize = 80.0f;

        char mDirectoryInputBuffer[256];

        String mContentItemDragDropString = "Content Browser Item Drag Drop";

        ImVec2 mMouseSelectionStart, mMouseSelectionEnd;
        bool mMouseSelectionStarted;

        bool mWindowHovered = false;

        static constexpr const char* sPopupID = "ContentBrowserPanel RightClickMenu";
    };

}
