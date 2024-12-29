#pragma once
#include <string>
#include <Windows.h>

#include "Base/Types.hpp"

namespace Cosmic::WindowsUtils
{
    void OutputWindowsErrorCode(DWORD dwErrorCode, ::Cosmic::StringView logMod, ::Cosmic::StringView file, ::Cosmic::StringView func, ::Cosmic::int32 line);
}

#if 1
#define CS_WINDOWS_CALL(func, __VA_ARGS__)\
do\
{\
    func;\
    auto dwErrorCode = GetLastError();\
    if (dwErrorCode != 0 && dwErrorCode != 997 && dwErrorCode != 2 && dwErrorCode != 10035 && dwErrorCode != 1400 && dwErrorCode != 127 && dwErrorCode != 181)\
    {\
        ::Cosmic::WindowsUtils::OutputWindowsErrorCode(dwErrorCode, _CosmicModuleLogInfo::ModuleLogName(), __FILE__, __FUNCTION__, __LINE__);\
        CS_BREAK();\
    }\
} while(0)
#else
#define CS_WINDOWS_CALL(func, __VA_ARGS__) func
#endif
