#include "App/Event/Events.hpp"
#include "App/Event/FileSystemEvents.hpp"
#include "Base/Macros.hpp"
#include "cspch.hpp"

#ifdef CS_PLATFORM_LINUX

#include "App/FileSystem.hpp"
#include "LinuxUtils.hpp"

#include <sys/inotify.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

CS_MODULE_LOG_INFO(Cosmic, Impl.OS.Linux.LinuxFileSystem)

namespace Cosmic
{

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
        int32 fd = inotify_init();
        int32 wd = inotify_add_watch(fd, mFileSystemWatcherDirectory.c_str(), IN_MODIFY | IN_CREATE | IN_DELETE);

        constexpr size_t bufferLength = 1024 * (sizeof(struct inotify_event) + 16);
        uint8 buffer[bufferLength];

        // temp
        while (true)
        {
            int i = 0;
            int length = read(fd, buffer, bufferLength);

            while (i < length)
            {
                inotify_event* e = (inotify_event*)&buffer[i];
                if (e->len)
                {
                    if (e->mask & IN_CREATE)
                    {
                        if (e->mask & IN_ISDIR)
                            EventSystem::AddEvent(new DirectoryAddedEvent(e->name));
                        else
                            EventSystem::AddEvent(new FileAddedEvent(File(e->name)));
                    }
                    else if (e->mask & IN_DELETE)
                    {
                        if (e->mask & IN_ISDIR)
                            EventSystem::AddEvent(new DirectoryRemovedEvent(e->name));
                        else
                            EventSystem::AddEvent(new FileRemovedEvent(File(e->name)));
                    }
                    else if (e->mask & IN_MODIFY)
                    {
                        if (e->mask & IN_ISDIR)
                            EventSystem::AddEvent(new DirectoryModifiedEvent(e->name));
                        else
                            EventSystem::AddEvent(new FileModifiedEvent(File(e->name)));
                    }
                }

                i += sizeof(inotify_event) + e->len;  
            }
        }

        inotify_rm_watch(fd, wd);
    }

    FilesAndDirectoriesInDirectory FileSystem::GetAllFilesAndDirectoriesInDirectory(const Directory& parentDir)
    {
        CS_NOT_IMPLEMENTED();

        return {};
    }

    Vector<String> FileSystem::ListDirectoryContents(const Directory& parentDir)
    {
        Vector<String> result = {};

        DIR* dir = opendir(parentDir.c_str());
        dirent* ent;
        
        if (dir != NULL)
        {
            while ((ent = readdir(dir)) != NULL)
            {
                if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name ,"..") == 0)
                    continue;

                result.emplace_back(ent->d_name);
            }
        }

        closedir(dir);

        return result;
    }

    bool FileSystem::IsFileOrDirectory(const String& path)
    {
        struct stat path_stat;
        stat(path.c_str(), &path_stat);
        
        return S_ISREG(path_stat.st_mode);
    }

    void FileSystem::CreateDirectory(const Directory& dir)
    {
        CS_NOT_IMPLEMENTED();
    }

    void FileSystem::RemoveDirectory(const Directory& dir)
    {
        CS_NOT_IMPLEMENTED();
    }

    void FileSystem::RenameDirectory(const Directory& dir, const Directory& newDir)
    {
        CS_NOT_IMPLEMENTED();
    }

    File FileSystem::CreateFile(const std::string_view absolutePath)
    {
        CS_NOT_IMPLEMENTED();
        return File(""); 
    }

    void FileSystem::RemoveFile(const File& file)
    {
        CS_NOT_IMPLEMENTED();
    }

    void FileSystem::CopyFile(const File& file, const StringView path)
    {
        CS_NOT_IMPLEMENTED();
    }

    void FileSystem::MoveFile(const File& file, const Directory& path)
    {
        CS_NOT_IMPLEMENTED();
    }

    void FileSystem::RenameFile(const File& file, const StringView nameAndExt)
    {
        CS_NOT_IMPLEMENTED();
    }

    void FileSystem::RenameFileExtension(const File& file, const StringView ext)
    {
        CS_NOT_IMPLEMENTED();
    }

    void FileSystem::RenameFileName(const File& file, const StringView name)
    {
        CS_NOT_IMPLEMENTED();
    }

}

#endif
