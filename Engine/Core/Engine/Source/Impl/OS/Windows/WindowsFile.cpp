#include "cspch.hpp"

#ifdef CS_PLATFORM_WINDOWS

#include "App/File.hpp"
#include "WindowsUtils.hpp"

#include <filesystem>

#include <Windows.h>
#include <windows.h>

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

    const String File::GetName()
    {
        return std::filesystem::path(mAbsolutePath.GetString()).filename().string();
    }

    const String File::GetExtension()
    {
        return std::filesystem::path(mAbsolutePath.GetString()).extension().string();
    }

    const String File::GetNameAndExtension()
    {
        return std::filesystem::path(mAbsolutePath.GetString()).root_name().string();
    }

    const Path File::GetAbsolutePath() const
    {
        return mAbsolutePath;
    }

    const size_t File::GetSize()
    {
        // TODO: Make this work

        HANDLE      hFile;
        DWORD       fileSize = 0;
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
        DWORD      dwNumberOfBytesRead = 0;
        char       readBuffer[BUFFER_SIZE] = { 0 };
        OVERLAPPED ol = { 0 };

        // get the file handle and open the file

        CS_WINDOWS_CALL(hFile = CreateFileA(
            mAbsolutePath.GetString().c_str(),            // file to open
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

    const uint8* File::ReadBinary()
    {
        // TODO: Implement
        return nullptr;
    }

    void File::Write(const std::string_view text)
    {
        HANDLE     hFile;

        // get the file handle and open the file

        CS_WINDOWS_CALL(hFile = CreateFileA(
            mAbsolutePath.GetString().c_str(),            // file to open
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

    void File::WriteBinary(const uint8* data, size_t size)
    {
        // TODO: Implement
    }

}

#endif
