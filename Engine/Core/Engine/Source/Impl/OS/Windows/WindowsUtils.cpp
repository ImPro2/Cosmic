#include "cspch.hpp"

#ifdef CS_PLATFORM_WINDOWS

#include "WindowsUtils.hpp"

#include "App/Log/Log.hpp"
#include "Base/Types.hpp"

namespace Cosmic::WindowsUtils
{
    void OutputWindowsErrorCode(DWORD dwErrorCode, ::Cosmic::StringView logMod, ::Cosmic::StringView file, ::Cosmic::StringView func, ::Cosmic::int32 line)
    {
        CS_PROFILE_FN();

        char* pMsgBuf = nullptr;

        const DWORD nMsgLen = FormatMessageA(
            FORMAT_MESSAGE_ALLOCATE_BUFFER |
            FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
            nullptr, dwErrorCode, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            reinterpret_cast<LPSTR>(&pMsgBuf), 0, nullptr
        );

        std::string errorString = pMsgBuf;
        LocalFree(pMsgBuf);

        if (nMsgLen == 0)
        {
            ::Cosmic::Log::Error("[Cosmic.{}]: Windows Error in [{}:{}:{}].", "Default");
            ::Cosmic::Log::Error("[Cosmic.{}]: Reason: `Unidentified Error Code`", "Default");
        }
        else
        {
            ::Cosmic::Log::Error("[Cosmic.{}]: Windows Error in [{}:{}:{}].", "Default", logMod, file, func, line);
            ::Cosmic::Log::Error("[Cosmic.{}]: Reason: `{}`", "Default", logMod, errorString.c_str());
        }

    }
}

#endif
