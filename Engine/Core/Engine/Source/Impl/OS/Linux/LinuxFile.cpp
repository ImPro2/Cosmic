#include "App/Log/LogMacros.hpp"
#include "Base/Macros.hpp"
#include "cspch.hpp"

#ifdef CS_PLATFORM_LINUX

#include "App/File.hpp"
#include "LinuxUtils.hpp"

#include <filesystem>

#include <stdio.h>
#include <sys/stat.h>

CS_MODULE_LOG_INFO(Cosmic, Impl.OS.Linux.LinuxFile);

namespace Cosmic
{

    const String File::GetName() const
    {
        //const StringView nameAndExt = GetNameAndExtension();
        //return StringView(nameAndExt.data(), nameAndExt.find('.'));
        return std::filesystem::path(mAbsolutePath).filename();
    }

    const String File::GetExtension() const
    {
        //const StringView nameAndExt = GetNameAndExtension();
        //return nameAndExt.substr(nameAndExt.find('.') + 1, nameAndExt.size() - 1);
        
        return std::filesystem::path(mAbsolutePath).extension();
    }

    const String File::GetNameAndExtension() const
    {
        /*if (mAbsolutePath.find('/') != String::npos)
        {
            size_t slashPos = mAbsolutePath.find_last_of('/') + 1;

            StringView sv = StringView(mAbsolutePath.c_str() + slashPos, mAbsolutePath.size() - slashPos);

            return mAbsolutePath.substr(slashPos);
        }*/

        return std::filesystem::path(mAbsolutePath).root_name();
    }
    
    const Path File::GetAbsolutePath() const
    {
        return mAbsolutePath;
    }
    
    const size_t File::GetSize()
    {
        struct stat f_stat;
        stat(mAbsolutePath.GetString().c_str(), &f_stat);
        size_t size = f_stat.st_size;

        return size;
    }
    
    const String File::Read()
    {
        FILE* f = fopen(mAbsolutePath.GetString().c_str(), "r");

        fseek(f, 0, SEEK_END);
        size_t size = (size_t)ftell(f);
        fseek(f, 0, 0);

        String buffer;
        fread((void*)buffer.c_str(), sizeof(char), size, f);

        fclose(f);
        return buffer;
    }

    const uint8* File::ReadBinary()
    {
        FILE* f = fopen(mAbsolutePath.GetString().c_str(), "rb");

        fseek(f, 0, SEEK_END);
        size_t size = (size_t)ftell(f);
        fseek(f, 0, 0);

        uint8_t* buffer;
        fread(buffer, sizeof(char), size, f);

        fclose(f);

        return buffer;
    }

    void File::Write(const StringView text)
    {
        FILE* f = fopen(mAbsolutePath.GetString().c_str(), "w");
    
        CS_ASSERT(f, "Invalid file path");

        fwrite(text.data(), sizeof(char), text.size(), f);
        fclose(f); 
    }

    void File::WriteBinary(const uint8* data, size_t size)
    {
        FILE* f = fopen(mAbsolutePath.GetString().c_str(), "wb");
    
        CS_ASSERT(f, "Invalid file path");

        fwrite(data, sizeof(uint8), size, f);
        fclose(f);
    }

}

#endif
