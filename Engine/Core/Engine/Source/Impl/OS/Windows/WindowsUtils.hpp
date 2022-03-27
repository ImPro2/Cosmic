#pragma once
#include <string>
#include <Windows.h>

import Cosmic.Base.Types;

namespace Cosmic::WindowsUtils
{
    void OutputWindowsErrorCode(DWORD dwErrorCode, ::Cosmic::StringView logMod, ::Cosmic::StringView file, ::Cosmic::StringView func, ::Cosmic::int32 line);
}

#define CS_WINDOWS_CALL(func, __VA_ARGS__)\
do\
{\
    func;\
    auto dwErrorCode = GetLastError();\
    if (dwErrorCode != 0 && dwErrorCode != 997 && dwErrorCode != 2)\
    {\
        CS_LOG_ERROR(__VA_ARGS__);\
        ::Cosmic::WindowsUtils::OutputWindowsErrorCode(dwErrorCode, _CosmicModuleLogInfo::ModuleLogName(), __FILE__, __FUNCTION__, __LINE__);\
        CS_BREAK();\
    }\
} while(0)