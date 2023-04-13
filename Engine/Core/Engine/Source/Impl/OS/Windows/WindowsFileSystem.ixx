module;
#include "cspch.hpp"
#include "WindowsUtils.hpp"

#include <Windows.h>
#include <windows.h>

export module Cosmic.Impl.OS.Windows.WindowsFileSystem;

import Cosmic.App.FileSystem;
import Cosmic.App.File;
import Cosmic.App.FileSystemEvents;
import Cosmic.App.Log;
import Cosmic.Base.Cast;
import Cosmic.Base.Types;
import Cosmic.App.Application;

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

    void FileSystem::Init(const Directory& fileSystemWatcherPath)
    {
        mFileSystemWatcherDirectory = fileSystemWatcherPath;
        mFileSystemWatcherThread = std::thread(&FileSystem::FileSystemWatcherThread);
    }

    void FileSystem::Shutdown()
    {

    }


    void FileSystem::FileSystemWatcherThread()
    {
        HANDLE dirHandle = CreateFileA(mFileSystemWatcherDirectory.c_str(), GENERIC_READ | FILE_LIST_DIRECTORY,
            FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
            NULL, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED,
            NULL
        );

        int flags = 0;
        flags |= FILE_NOTIFY_CHANGE_FILE_NAME;
        flags |= FILE_NOTIFY_CHANGE_DIR_NAME;
        flags |= FILE_NOTIFY_CHANGE_ATTRIBUTES;
        flags |= FILE_NOTIFY_CHANGE_SIZE;
        flags |= FILE_NOTIFY_CHANGE_LAST_WRITE;
        flags |= FILE_NOTIFY_CHANGE_LAST_ACCESS;
        flags |= FILE_NOTIFY_CHANGE_CREATION;
        flags |= FILE_NOTIFY_CHANGE_SECURITY;

        char filename[100];
        char buffer[2048];
        DWORD bytesReturned;
        FILE_NOTIFY_INFORMATION* pNotify;
        int offset = 0;
        OVERLAPPED pollingOverlap;
        pollingOverlap.OffsetHigh = 0;
        pollingOverlap.hEvent = CreateEventA(NULL, TRUE, FALSE, NULL);

        bool result = true;
        HANDLE hEvents[2];
        hEvents[0] = pollingOverlap.hEvent;
        hEvents[1] = CreateEventA(NULL, TRUE, FALSE, NULL);

        while (result)
        {
            result = ReadDirectoryChangesW(
                dirHandle,
                &buffer,
                sizeof(buffer),
                true,
                flags,
                &bytesReturned,
                &pollingOverlap,
                NULL
            );

            DWORD event = WaitForMultipleObjects(2, hEvents, FALSE, INFINITE);
            offset = 0;
            int rename = 0;

            if (event == WAIT_OBJECT_0 + 1)
            {
                break;
            }

            do
            {
                pNotify = (FILE_NOTIFY_INFORMATION*)((char*)buffer + offset);
                strcpy(filename, "");
                int filenamelen = WideCharToMultiByte(CP_ACP, 0, pNotify->FileName, pNotify->FileNameLength / 2, filename, sizeof(filename), NULL, NULL);
                filename[pNotify->FileNameLength / 2] = '\0';

                File file = File(filename);

                switch (pNotify->Action)
                {
                    case FILE_ACTION_ADDED:
                    {
                        Application::Get()->OnEvent(FileAddedEvent(file));
                        break;
                    }
                    case FILE_ACTION_REMOVED:
                    {
                        Application::Get()->OnEvent(FileRemovedEvent(file));
                        break;
                    }
                    case FILE_ACTION_MODIFIED:
                    {
                        Application::Get()->OnEvent(FileModifiedEvent(file));
                        break;
                    }
                    case FILE_ACTION_RENAMED_NEW_NAME:
                    {
                        Application::Get()->OnEvent(FileRenamedEvent(file));
                        break;
                    }
                    default:
                    {
                        CS_LOG_INFO("asdf");
                        break;
                    }
                }

                offset += pNotify->NextEntryOffset;

            } while (pNotify->NextEntryOffset);
        }

        CloseHandle(dirHandle);
    }


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

    bool FileSystem::IsFileOrDirectory(const String& path)
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

    void FileSystem::CopyFile(const File& file, const StringView path)
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

    void FileSystem::RenameFile(const File& file, const StringView newName)
    {
        std::string newFileName = std::vformat("{}/{}", std::make_format_args(((File)file).GetParentDirectory().data(), newName.data()));
        ((File)file).SetAbsolutePath(std::string_view(newFileName));

        CS_WINDOWS_CALL(::MoveFileA(
            ((File)file).GetAbsolutePath().data(), // File to be renamed
            newFileName.c_str()                    // New name for the file.
        ), "Unable to rename file.");
    }

    void FileSystem::RenameFileExtension(const File& file, const StringView ext)
    {
        std::string newName = std::vformat("{}{}", std::make_format_args(((File)file).GetName().data(), ext.data()));
        RenameFile(file, std::string_view(newName));
    }

    void FileSystem::RenameFileName(const File& file, const StringView name)
    {
        std::string newName = std::vformat("{}{}", std::make_format_args(name.data(), ((File)file).GetExtension()));
        RenameFile(file, std::string_view(newName));
    }

}