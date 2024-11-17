#pragma once
#include "Panels.hpp"

#include "App/Path.hpp"
#include "App/Event/FileSystemEvents.hpp"

namespace Cosmic
{

    class ContentBrowserPanel : public Panel
    {
    public:
        ContentBrowserPanel();

    public:
        void OnInit() override;
        void OnEvent(const Event& e) override;
        void OnImGuiRender() override;

    private:
        void UpdateContents();

    private:
        void RenderTop();
        void RenderContents();
        void RenderContentItem(const Path& path);

    private:
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
        float32 mContentItemSize = 80.0f;
    };

}
