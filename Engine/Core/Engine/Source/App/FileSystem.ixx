module;
#include "cspch.hpp"
export module Cosmic.App.FileSystem;

namespace Cosmic
{

    export using Directory = std::string;
    class FileSystem;
    class File;

    export class File
    {
    public:
        const std::string_view GetName();
        const std::string_view GetExtension();
        const std::string_view GetNameAndExtension();

        const std::string_view GetAbsolutePath();
        const std::string_view GetParentDirectory();

        const size_t           GetSize();

        const std::string      Read();
        const unsigned char*   ReadBinary();

        void                   Write(const std::string_view text);
        void                   WriteBinary(const unsigned char* text);

        File(const std::string_view absolutePath)
            : mAbsolutePath(absolutePath)
        {
        }
    
    private:
        void SetAbsolutePath(const std::string_view absolutePath)
        {
            mAbsolutePath = absolutePath;
        }

    private:
        std::string mAbsolutePath;

    private:
        friend class FileSystem;
    };

    export struct FilesAndDirectoriesInDirectory
    {
        std::vector<File>      files;
        std::vector<Directory> dirctories;
    };

    export class FileSystem
    {
    public:
        static FilesAndDirectoriesInDirectory GetAllFilesAndDirectoriesInDirectory(const Directory& parentDir);
        static bool                           IsFileOrDirectory(const std::string& path); // returns true for a file, false for directory

        static void CreateDirectory(const Directory& dir);
        static void RemoveDirectory(const Directory& dir);
        static void RenameDirectory(const Directory& dir, const Directory& newDir);

        static File CreateFile(         const std::string_view absolutePath);
        static void RemoveFile(         const File&            file);
        static void CopyFile(           const File&            file, const std::string_view path);
        static void MoveFile(           const File&            file, const Directory&       path);
        static void RenameFile(         const File&            file, const std::string_view nameAndExt);
        static void RenameFileExtension(const File&            file, const std::string_view ext);
        static void RenameFileName(     const File&            file, const std::string_view name);
    };

}