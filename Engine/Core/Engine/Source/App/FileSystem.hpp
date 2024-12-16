#pragma once
#include <thread>

#include "Base/Types.hpp"
#include "App/Path.hpp"
#include "App/File.hpp"

#include "Memory/SmartPtrs.hpp"

namespace Cosmic
{

    class Application;

    class FileSystem : public IRefCounted
    {
    private:
        static PersistentRef<FileSystem> Init(const Path& fileSystemWatcherPath);
        static void                      Shutdown();

    public:
        static Path GetExecutableDirectory();
        static Path GetCurrentWorkingDirectory();

        static Vector<Path> ListDirectoryContents(const Path& parentDir);

        static bool IsFile(const Path& path);
        static bool IsDirectory(const Path& path);
        static bool Exists(const Path& path);

        static Path GetParentDirectory(const Path& path);

        static void CreateDirectory(const Path& dir);
        static void RemoveDirectory(const Path& dir);
        static void RenameDirectory(const Path& dir, const Path& newDir);

        static File CreateFile(const Path& absolutePath);
        static void RemoveFile(const File& file);
        static void CopyFile(const File& file, const Path& path);
        static void MoveFile(const File& file, const Path& path);
        static void RenameFile(const File& file, const String& nameAndExt);

    private:
        static void FileSystemWatcherThread();

    private:
        std::thread mFileSystemWatcherThread;
        Path        mFileSystemWatcherDirectory;

        inline static PersistentRef<FileSystem> sInstance;

        friend class Application;
    };

}
