#pragma once

#include "App/Module.hpp"
#include "App/File.hpp"
#include "App/FileSystem.hpp"

#include <functional>
#include <initializer_list>

namespace Cosmic
{

    using FileDialogCallback = std::function<void(File f)>;

    class FileDialogModule : public Module
    {
    public:
        FileDialogModule(const String& dir);
    
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
        void SetDirectory(const String& newDir);
        void CloseDialog(bool cancelled = false);

    private:
        enum class EDialogMode
        {
            Open, Save
        };

    private:
        String mDirectory;
        FileDialogCallback mCallback;
        EDialogMode mDialogMode;

        Vector<String> mOpenFileFilters = {};
        String mOpenFileDescription;

        String mDefaultSaveFilename;
        Vector<String> mSaveFiletypes;

        String mWindowName = "";

        String mSelectedPath;
        bool mClosedDialog = false;

        char mDirectoryInputBuffer[256], mFileInputBuffer[256], mSearchInputBuffer[256];
        Vector<String> mDirectoryContents;
        int32 mSelectedItemIndex;
    };
    
}
