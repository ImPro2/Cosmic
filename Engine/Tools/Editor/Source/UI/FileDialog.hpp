#pragma once

#include "App/Module.hpp"
#include "App/File.hpp"
#include "App/FileSystem.hpp"

#include <functional>
#include <initializer_list>

namespace Cosmic
{

    using FileDialogCallback = std::function<void(File f)>;

    class FileDialogModule : public IModule
    {
    public:
        // Defaults to cwd/Engine/Tools/Editor
        FileDialogModule(const Path& dir = "");
    
    public:
        void OnInit() override;
        void OnImGuiRender() override;

    public:
        void SetOpenFileCallback(const String& description, const Vector<String>& filters, FileDialogCallback callback);
        void SetSaveFileCallback(const String& defaultSaveFilename, const Vector<String>& filetypes, FileDialogCallback callback);

    private:
        void RenderTop();
        void RenderDirectoryTree();
        void RenderOpenFileWidgets();
        void RenderSaveFileWidgets();
        
    private:
        void SetDirectory(const Path& newDir);
        void CloseDialog(bool cancelled = false);

    private:
        enum class EDialogMode
        {
            Open, Save
        };

    private:
        Path mDirectory;
        FileDialogCallback mCallback;
        EDialogMode mDialogMode;

        Vector<String> mOpenFileFilters = {};
        String mOpenFileDescription;

        String mDefaultSaveFilename;
        Vector<String> mSaveFiletypes;

        String mWindowName = "";

        Path mSelectedPath;
        bool mClosedDialog = false;

        char mDirectoryInputBuffer[256], mFileInputBuffer[256], mSearchInputBuffer[256];
        Vector<Path> mDirectoryContents;
        int32 mSelectedItemIndex;
    };
    
}
