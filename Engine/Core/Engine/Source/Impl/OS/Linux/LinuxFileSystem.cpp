#include "Event/Events.hpp"
#include "Event/Type/FileSystemEvents.hpp"
#include "Base/Macros.hpp"
#include "cspch.hpp"
#include <climits>
#include <filesystem>

#ifdef CS_PLATFORM_LINUX

#include "App/FileSystem.hpp"
#include "LinuxUtils.hpp"

#include <fcntl.h>
#include <unistd.h>
#include <limits.h>
#include <sys/sendfile.h>
#include <sys/inotify.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

CS_MODULE_LOG_INFO(Cosmic, Impl.OS.Linux.LinuxFileSystem)

namespace Cosmic
{

    void FileSystem::Init(const Path& fileSystemWatcherPath)
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
        int32 wd = inotify_add_watch(fd, mFileSystemWatcherDirectory.GetString().c_str(), IN_MODIFY | IN_CREATE | IN_DELETE);

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
                            EventSystem::AddEvent(new DirectoryAddedEvent(String(e->name)));
                        else
                            EventSystem::AddEvent(new FileAddedEvent(File(Path(e->name))));
                    }
                    else if (e->mask & IN_DELETE)
                    {
                        if (e->mask & IN_ISDIR)
                            EventSystem::AddEvent(new DirectoryRemovedEvent(String(e->name)));
                        else
                            EventSystem::AddEvent(new FileRemovedEvent(File(Path(e->name))));
                    }
                    else if (e->mask & IN_MODIFY)
                    {
                        if (e->mask & IN_ISDIR)
                            EventSystem::AddEvent(new DirectoryModifiedEvent(String(e->name)));
                        else
                            EventSystem::AddEvent(new FileModifiedEvent(File(Path(e->name))));
                    }
                }

                i += sizeof(inotify_event) + e->len;  
            }
        }

        inotify_rm_watch(fd, wd);
    }

    Vector<Path> FileSystem::ListDirectoryContents(const Path& parentDir)
    {
        Vector<Path> result = {};

        DIR* dir = opendir(parentDir.GetString().c_str());
        dirent* ent;
        
        if (dir != NULL)
        {
            while ((ent = readdir(dir)) != NULL)
            {
                if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name ,"..") == 0)
                    continue;

                result.emplace_back(parentDir / String(ent->d_name));
            }
        }

        closedir(dir);

        return result;
    }

    bool FileSystem::IsFile(const Path& path)
    {
        struct stat path_stat;
        stat(path.GetString().c_str(), &path_stat);
        
        return S_ISREG(path_stat.st_mode);
    }

    bool FileSystem::IsDirectory(const Path& path)
    {
        struct stat path_stat;
        stat(path.GetString().c_str(), &path_stat);
        return S_ISDIR(path_stat.st_mode);
    }

    bool FileSystem::Exists(const Path& path)
    {
        struct stat path_stat;
        return (stat(path.GetString().c_str(), &path_stat) == 0);
    }

    Path FileSystem::GetParentDirectory(const Path& path)
    {
        return std::filesystem::path(path.GetString()).parent_path().string();
    }

    void FileSystem::CreateDirectory(const Path& dir)
    {
        struct stat path_stat = { 0 };

        // If the dir does not exist yet, create it.
        if (stat(dir.GetString().c_str(), &path_stat) == -1)
            CS_ASSERT(mkdir(dir.GetString().c_str(), S_IRWXU | S_IRWXG | S_IRWXO) != -1, "Failed to create directory");
    }

    void FileSystem::RemoveDirectory(const Path& dir)
    {
        CS_ASSERT(rmdir(dir.GetString().c_str()) == 0, "Failed to remove directory");
    }

    void FileSystem::RenameDirectory(const Path& dir, const Path& newDir)
    {
        CS_NOT_IMPLEMENTED();
    }

    File FileSystem::CreateFile(const Path& absolutePath)
    {
        CS_ASSERT(open(absolutePath.GetString().c_str(), O_CREAT | O_RDWR, S_IRUSR | S_IRGRP | S_IROTH), "Failed to create file.");

        return File(absolutePath);
    }

    void FileSystem::RemoveFile(const File& file)
    {
        CS_ASSERT(remove(file.GetAbsolutePath().GetString().c_str()) == 0, "Failed to remove file.");
    }

    void FileSystem::CopyFile(const File& file, const Path& path)
    {
        const String& srcStr = file.GetAbsolutePath().GetString();
        const String  dstStr = path / file;

        // https://stackoverflow.com/questions/2180079/how-can-i-copy-a-file-on-unix-using-c

        int input, output;

        CS_ASSERT(open(srcStr.c_str(), O_RDONLY) == 0, "Failed to open src file.");
        CS_ASSERT(creat(dstStr.c_str(), 0660) == 0, "Failed to open dst file.");

        struct stat file_stat = { 0 };
        int result = fstat(input, &file_stat);
        
        off_t copied = 0;

        while (result == 0 && copied < file_stat.st_size)
        {
            ssize_t written = sendfile(output, input, &copied, SSIZE_MAX);
            copied += written;

            if (written == -1)
                result = -1;
        }

        close(input);
        close(output);
    }

    void FileSystem::MoveFile(const File& file, const Path& path)
    {
        const String& srcStr = file.GetAbsolutePath().GetString();
        const String& dstStr = path.GetString();
        
        CS_ASSERT(rename(srcStr.c_str(), dstStr.c_str()) == 0, "Failed to move file.");
    }

    void FileSystem::RenameFile(const File& file, const String& nameAndExt)
    {
        const String& srcStr = file.GetAbsolutePath().GetString();
        const String  dstStr = GetParentDirectory(file.GetAbsolutePath()) / nameAndExt;
        
        CS_ASSERT(rename(srcStr.c_str(), dstStr.c_str()) == 0, "Failed to rename file.");
    }

}

#endif
