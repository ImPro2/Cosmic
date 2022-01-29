module;
#include "cspch.hpp"
#include "WindowsUtils.hpp"

#include <Windows.h>
#include <windows.h>

export module Cosmic.Impl.OS.Windows.WindowsFileSystem;

import Cosmic.App.FileSystem;
import Cosmic.App.Log;
import Cosmic.Base.Cast;
import Cosmic.Base.Types;

CS_MODULE_LOG_INFO(Cosmic, Impl.OS.Windows.WindowsFileSystem);

#undef MoveFile
#undef CopyFile
#undef CreateDirectory
#undef RemoveDirectory
#undef CreateFile


namespace Cosmic
{

    namespace Utils
    {
        
        // https://en.cppreference.com/w/cpp/string/basic_string/replace
        std::size_t ReplaceAll(std::string& inout, std::string_view what, std::string_view with)
        {
            std::size_t count{};
            for (std::string::size_type pos{};
                inout.npos != (pos = inout.find(what.data(), pos, what.length()));
                pos += with.length(), ++count) {
                inout.replace(pos, what.length(), with.data(), with.length());
            }
            return count;
        }

    }


    /********************************************************
    *                         File                          *
    ********************************************************/

    const std::string_view File::GetName()
    {
        const std::string_view nameAndExt = GetNameAndExtension();

        return nameAndExt.substr(0, nameAndExt.find('.'));
    }

    const std::string_view File::GetExtension()
    {
        const std::string_view nameAndExt = GetNameAndExtension();

        return nameAndExt.substr(nameAndExt.find('.') + 1, nameAndExt.size() - 1);
    }

    const std::string_view File::GetNameAndExtension()
    {
        return std::string_view(mAbsolutePath.c_str() + mAbsolutePath.find_last_of('/') + 1);
    }

    const std::string_view File::GetAbsolutePath()
    {
        return mAbsolutePath;
    }

    const std::string_view File::GetParentDirectory()
    {
        // TODO: Do extra checks and stuff

        std::string_view s(mAbsolutePath.c_str(), mAbsolutePath.find_last_of('/'));
        return s;
    }

    const size_t File::GetSize()
    {
        // TODO: Make this work

        HANDLE      hFile;
        DWORD       fileSize     = 0;
        std::string absolutePath = mAbsolutePath;

        // Ensure that windows can read the file path correctly

        Utils::ReplaceAll(absolutePath, "/", "\\");

        // Obtain the file handle

        CS_WINDOWS_CALL(hFile = ::CreateFileA(
            absolutePath.c_str(),                         // file to open
            GENERIC_READ,                                 // open for reading
            FILE_SHARE_READ,                              // share for reading
            NULL,                                         // default security
            OPEN_EXISTING,                                // existing file only
            FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, // normal file
            NULL                                          // no attr. template
        ), "Unable to open file ``.");

        // Get the file size

        CS_WINDOWS_CALL(::GetFileSize(
            hFile,
            &fileSize
        ), "Failed to get the desired file size of file `{}`", mAbsolutePath);

        // Close the file handle

        ::CloseHandle(hFile);

        // Return the file size

        return (size_t)fileSize;
    }

#define BUFFER_SIZE 1024

    const std::string File::Read()
    {
        HANDLE     hFile;
        DWORD      dwNumberOfBytesRead     =   0;
        char       readBuffer[BUFFER_SIZE] = { 0 };
        OVERLAPPED ol                      = { 0 };

        // get the file handle and open the file

        CS_WINDOWS_CALL(hFile = CreateFileA(
            mAbsolutePath.c_str(),                        // file to open
            GENERIC_READ,                                 // open for reading
            FILE_SHARE_READ,                              // share for reading
            NULL,                                         // default security
            OPEN_EXISTING,                                // existing file only
            FILE_ATTRIBUTE_NORMAL,                        // normal file
            NULL                                          // no attr. template
        ), "Unable to open file ``.");

        // read the file

        CS_WINDOWS_CALL(ReadFile(
            hFile,                // file to open
            readBuffer,           // output pointer containing file contents
            BUFFER_SIZE - 1,      // number of bytes to read
            &dwNumberOfBytesRead, // number of bytes read
            &ol                   // output of LPOVERLAPPED
        ), "Unable to read file ``.");

        // insert a NULL character

        if (dwNumberOfBytesRead > 0 && dwNumberOfBytesRead < BUFFER_SIZE)
        {
            readBuffer[dwNumberOfBytesRead] = '\0';
        }

        // close the file

        ::CloseHandle(hFile);

        // return

        return std::string(readBuffer);
    }

    const unsigned char* File::ReadBinary()
    {
        // TODO: Implement
        return nullptr;
    }

    void File::Write(const std::string_view text)
    {
        HANDLE     hFile;

        // get the file handle and open the file

        CS_WINDOWS_CALL(hFile = CreateFileA(
            mAbsolutePath.c_str(),                        // file to open
            GENERIC_WRITE,                                // open for writing
            0,                                            // do not share for writing
            NULL,                                         // default security
            OPEN_EXISTING,                                // existing file only
            FILE_ATTRIBUTE_NORMAL,                        // normal file
            NULL                                          // no attr. template
        ), "Unable to open file ``.");

        // write to the file

        CS_WINDOWS_CALL(WriteFile(
            hFile,
            text.data(),
            strlen(text.data()),
            nullptr,
            NULL
        ), "Unable to write to file ``");

        ::CloseHandle(hFile);
    }

    void File::WriteBinary(const unsigned char* text)
    {
        // TODO: Implement
    }


    /*************************************************************
    *                         FileSystem                         *
    *************************************************************/

    FilesAndDirectoriesInDirectory FileSystem::GetAllFilesAndDirectoriesInDirectory(const Directory& parentDir)
    {
        HANDLE                           hFind;
        WIN32_FIND_DATAA                 ffd;
        std::string                      parentDirAsString = (std::string)parentDir;
        FilesAndDirectoriesInDirectory   result;

        Utils::ReplaceAll(parentDirAsString, "/", "\\");
        parentDirAsString += "\\*";

        CS_WINDOWS_CALL(hFind = ::FindFirstFileA(
            parentDirAsString.c_str(),
            &ffd
        ), "First file could not be found in directory.");

        std::string finalParentDir = (std::string)(parentDir + '/');

        do 
        {
            std::string name = ffd.cFileName;
            std::string absolutePath = finalParentDir + '/' + name;

            if (name == "." || name == "..")
                continue;

            if (IsFileOrDirectory(ffd.cFileName))
                result.files.emplace_back(ffd.cFileName);
            else
                result.dirctories.emplace_back(ffd.cFileName);
        } while (::FindNextFileA(hFind, &ffd));

        return result;
    }

    bool FileSystem::IsFileOrDirectory(const std::string& path)
    {
        if (path.find('.') != std::string_view::npos)
            return true;
        return false;
    }

    void FileSystem::CreateDirectory(const Directory& dir)
    {
        CS_WINDOWS_CALL(::CreateDirectoryA(
            dir.c_str(),                     // The desired directory.
            nullptr                          // Security attributes.
        ), "Unable to create directory.");
    }

    void FileSystem::RemoveDirectory(const Directory& dir)
    {
        CS_WINDOWS_CALL(::RemoveDirectoryA(
            dir.c_str()                      // The desired directory.
        ), "Unable to remove directory.");
    }

    void FileSystem::RenameDirectory(const Directory& dir, const Directory& newDir)
    {
        CS_WINDOWS_CALL(::MoveFileA(
            dir.data(),                      // Directory to rename.
            newDir.data()                    // Directory that it will be renamed to.
        ), "Unable to rename the directory.");
    }

    File FileSystem::CreateFile(const std::string_view absolutePath)
    {
        HANDLE hFile;
        File   file(absolutePath);

        CS_WINDOWS_CALL(::CreateFileA(
            absolutePath.data(),             // Absolute path of the file.
            GENERIC_READ | GENERIC_WRITE,    // Access modes.
            0,                               // Shared.
            nullptr,                         // Security.
            CREATE_NEW,                      // Create a new file.
            FILE_ATTRIBUTE_NORMAL,           // Normal file.
            nullptr                          // Template file.
        ), "Failed to create new file.");

        return file;
    }

    void FileSystem::RemoveFile(const File& file)
    {
        CS_WINDOWS_CALL(::DeleteFileA(
            ((File)file).GetAbsolutePath().data()  // File to delete.
        ), "Unable to remove the file.");
    }

    void FileSystem::CopyFile(const File& file, const std::string_view path)
    {
        CS_WINDOWS_CALL(::CopyFileA(
            ((File)file).GetAbsolutePath().data(), // existing file to be copied.
            path.data(),                           // destination
            FALSE                                  // if the function should fail if another file already exists.
        ), "Failed to copy file.");
    }

    void FileSystem::MoveFile(const File& file, const Directory& path)
    {
        CS_WINDOWS_CALL(::MoveFileA(
            ((File)file).GetAbsolutePath().data(), // File that is going to be moved.
            path.c_str()                           // Destination of the file about to move.
        ), "Failed to move the file.");
    }

    void FileSystem::RenameFile(const File& file, const std::string_view newName)
    {
        std::string newFileName = std::vformat("{}/{}", std::make_format_args(((File)file).GetParentDirectory().data(), newName.data()));
        ((File)file).SetAbsolutePath(std::string_view(newFileName));

        CS_WINDOWS_CALL(::MoveFileA(
            ((File)file).GetAbsolutePath().data(), // File to be renamed
            newFileName.c_str()                    // New name for the file.
        ), "Unable to rename file.");
    }

    void FileSystem::RenameFileExtension(const File& file, const std::string_view ext)
    {
        std::string newName = std::vformat("{}{}", std::make_format_args(((File)file).GetName().data(), ext.data()));
        RenameFile(file, std::string_view(newName));
    }

    void FileSystem::RenameFileName(const File& file, const std::string_view name)
    {
        std::string newName = std::vformat("{}{}", std::make_format_args(name.data(), ((File)file).GetExtension()));
        RenameFile(file, std::string_view(newName));
    }

}