module;
#include "cspch.hpp"
export module Cosmic.App.FileSystem;

import Cosmic.Base.Types;

namespace Cosmic
{

    export using Directory = String;
    class FileSystem;
    class File;

    export class File
    {
    public:
        const StringView     GetName();
        const StringView     GetExtension();
        const StringView     GetNameAndExtension();

        const StringView     GetAbsolutePath();
        const StringView     GetParentDirectory();

        const size_t         GetSize();

        const String         Read();
        const unsigned char* ReadBinary();

        void                   Write(const StringView text);
        void                   WriteBinary(const unsigned char* text);

        File(const std::string_view absolutePath)
            : mAbsolutePath(absolutePath)
        {
        }
    
    private:
        void SetAbsolutePath(const StringView absolutePath)
        {
            mAbsolutePath = absolutePath;
        }

    private:
        String mAbsolutePath;

    private:
        friend class FileSystem;
    };

    export struct FilesAndDirectoriesInDirectory
    {
        Vector<File>      files;
        Vector<Directory> dirctories;
    };

    export class FileSystem
    {
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
    };

}