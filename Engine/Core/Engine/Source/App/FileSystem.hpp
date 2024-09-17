#pragma once
#include <thread>

#include "Base/Types.hpp"
#include "App/File.hpp"

namespace Cosmic
{

    using Directory = String;    

    struct FilesAndDirectoriesInDirectory
    {
        Vector<File>      files;
        Vector<Directory> dirctories;
    };

    class FileSystem
    {
    public:
        static void Init(const Directory& fileSystemWatcherPath);
        static void Shutdown();

    public:
        static FilesAndDirectoriesInDirectory GetAllFilesAndDirectoriesInDirectory(const Directory& parentDir);
        static bool                           IsFileOrDirectory(const String& path); // returns true for a file, false for directory

        static void CreateDirectory(const Directory& dir);
        static void RemoveDirectory(const Directory& dir);
        static void RenameDirectory(const Directory& dir, const Directory& newDir);

        static File CreateFile(         const std::string_view absolutePath);
        static void RemoveFile(         const File&            file);
        static void CopyFile(           const File&            file, const StringView path);
        static void MoveFile(           const File&            file, const Directory& path);
        static void RenameFile(         const File&            file, const StringView nameAndExt);
        static void RenameFileExtension(const File&            file, const StringView ext);
        static void RenameFileName(     const File&            file, const StringView name);

    private:
        static void FileSystemWatcherThread();

    private:
        inline static std::thread mFileSystemWatcherThread;
        inline static Directory   mFileSystemWatcherDirectory;
    };

}