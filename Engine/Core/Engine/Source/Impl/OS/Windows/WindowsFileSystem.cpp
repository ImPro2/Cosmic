#include "cspch.hpp"

#ifdef CS_PLATFORM_WINDOWS

#include "App/FileSystem.hpp"
#include "WindowsUtils.hpp"
#include <filesystem>

#include <Windows.h>
#include <windows.h>

#undef near
#undef far
#undef GetCurrentTime

#include "App/File.hpp"
#include "Event/Type/FileSystemEvents.hpp"
#include "App/Log/Log.hpp"
#include "Base/Cast.hpp"
#include "Base/Cast.hpp"
#include "Base/Types.hpp"
#include "App/Application.hpp"

#include <format>

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
        static std::size_t ReplaceAll(std::string& inout, std::string_view what, std::string_view with)
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

    PersistentRef<FileSystem> FileSystem::Init(const Path& fileSystemWatcherPath)
    {
        sInstance = CreatePersistentRef<FileSystem>();

        sInstance->mFileSystemWatcherDirectory = fileSystemWatcherPath;
        sInstance->mFileSystemWatcherThread    = std::thread(&FileSystem::FileSystemWatcherThread);

        return sInstance;
    }

    void FileSystem::Shutdown()
    {
        sInstance.Release();
    }

    void FileSystem::FileSystemWatcherThread()
    {
        HANDLE dirHandle = CreateFileA(sInstance->mFileSystemWatcherDirectory.GetString().c_str(), GENERIC_READ | FILE_LIST_DIRECTORY,
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

        char filename[1024];
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

                File file = File(GetCurrentWorkingDirectory() / Path(StringUtils::Replace(filename, '\\', '/')));

                switch (pNotify->Action)
                {
                    case FILE_ACTION_ADDED:
                    {
                        EventSystem::DeferEvent<FileAddedEvent>(file);
                        break;
                    }
                    case FILE_ACTION_REMOVED:
                    {
                        EventSystem::DeferEvent<FileRemovedEvent>(file);
                        break;
                    }
                    case FILE_ACTION_MODIFIED:
                    {
                        EventSystem::DeferEvent<FileModifiedEvent>(file);
                        break;
                    }
                    case FILE_ACTION_RENAMED_NEW_NAME:
                    {
                        EventSystem::DeferEvent<FileRenamedEvent>(file);
                        break;
                    }
                    default:
                    {
                        break;
                    }
                }

                offset += pNotify->NextEntryOffset;

            } while (pNotify->NextEntryOffset);
        }

        CloseHandle(dirHandle);
    }


    Path FileSystem::GetExecutableDirectory()
    {
        char buf[1024];

        GetModuleFileNameA(nullptr, buf, 1024);

        return String(buf);
    }

    Path FileSystem::GetCurrentWorkingDirectory()
    {
        String cwd = std::filesystem::current_path().string();
		Utils::ReplaceAll(cwd, "\\", "/");

        return cwd;
    }

    Vector<Path> FileSystem::ListDirectoryContents(const Path& parentDir)
    {
        HANDLE           hFind;
        WIN32_FIND_DATAA ffd;
        String           parentDirAsString = (String)parentDir;
        Vector<Path>     result;

        Utils::ReplaceAll(parentDirAsString, "/", "\\");
        parentDirAsString += "\\*";

        CS_WINDOWS_CALL(hFind = ::FindFirstFileA(
            parentDirAsString.c_str(),
            &ffd
        ), "First file could not be found in directory.");

        do 
        {
            String name = ffd.cFileName;

            if (name == "." || name == "..")
                continue;

            result.emplace_back(parentDir / name);
        } while (::FindNextFileA(hFind, &ffd));

        return result;
    }

    bool FileSystem::IsFile(const Path& path)
    {
        if (path.GetString().find('.') != std::string_view::npos)
            return true;
        return false;
    }

    bool FileSystem::IsDirectory(const Path& path)
    {
        return !IsFile(path);
    }

    bool FileSystem::Exists(const Path& path)
    {
        return (::GetFileAttributesA(path.GetString().c_str()) != INVALID_FILE_ATTRIBUTES);
    }

    Path FileSystem::GetParentDirectory(const Path& path)
    {
        return Path(std::filesystem::path(path.GetString()).parent_path().string());
    }

    void FileSystem::CreateDirectory(const Path& dir)
    {
        CS_WINDOWS_CALL(::CreateDirectoryA(
            dir.GetString().c_str(),         // The desired directory.
            nullptr                          // Security attributes.
        ), "Unable to create directory.");
    }

    void FileSystem::RemoveDirectory(const Path& dir)
    {
        CS_WINDOWS_CALL(::RemoveDirectoryA(
            dir.GetString().c_str()          // The desired directory.
        ), "Unable to remove directory.");
    }

    void FileSystem::RenameDirectory(const Path& dir, const Path& newDir)
    {
        CS_WINDOWS_CALL(::MoveFileA(
            dir.GetString().c_str(),         // Directory to rename.
            newDir.GetString().c_str()       // Directory that it will be renamed to.
        ), "Unable to rename the directory.");
    }

    File FileSystem::CreateFile(const Path& absolutePath)
    {
        HANDLE hFile;
        File   file(absolutePath);

        CS_WINDOWS_CALL(::CreateFileA(
            absolutePath.GetString().c_str(), // Absolute path of the file.
            GENERIC_READ | GENERIC_WRITE,     // Access modes.
            0,                                // Shared.
            nullptr,                          // Security.
            CREATE_NEW,                       // Create a new file.
            FILE_ATTRIBUTE_NORMAL,            // Normal file.
            nullptr                           // Template file.
        ), "Failed to create new file.");

        return file;
    }

    void FileSystem::RemoveFile(const File& file)
    {
        CS_WINDOWS_CALL(::DeleteFileA(
            file.GetAbsolutePath().GetString().c_str()  // File to delete.
        ), "Unable to remove the file.");
    }

    void FileSystem::CopyFile(const File& file, const Path& path)
    {
        CS_WINDOWS_CALL(::CopyFileA(
            file.GetAbsolutePath().GetString().c_str(), // existing file to be copied.
            path.GetString().c_str(),                   // destination
            FALSE                                       // if the function should fail if another file already exists.
        ), "Failed to copy file.");
    }

    void FileSystem::MoveFile(const File& file, const Path& path)
    {
        CS_WINDOWS_CALL(::MoveFileA(
            file.GetAbsolutePath().GetString().c_str(), // File that is going to be moved.
            path.GetString().c_str()                    // Destination of the file about to move.
        ), "Failed to move the file.");
    }

    void FileSystem::RenameFile(const File& file, const String& nameAndExt)
    {
        String prevNameAndExt = file.GetAbsolutePath().GetBase();
        String newNameAndExt  = GetParentDirectory(file.GetAbsolutePath()) / nameAndExt;

        CS_WINDOWS_CALL(::MoveFileA(
            prevNameAndExt.c_str(),           // File to be renamed
            newNameAndExt.c_str()             // New name for the file.
        ), "Unable to rename file.");
    }

}

#endif
