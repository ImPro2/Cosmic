#pragma once
#include <string>

namespace Cosmic
{

    template<typename From, typename To>
    To Cast(From from)
    {
        //static_assert(false);
    }

    inline std::wstring Cast(std::string s)
    {
        CS_PROFILE_FN();

        std::wstring to = L"";
        mbstowcs((wchar_t*)to.c_str(), s.c_str(), s.size());
        return to;
    }

    inline std::string Cast(std::wstring s)
    {
        CS_PROFILE_FN();

        std::string to = "";
        wcstombs((char*)to.c_str(), s.c_str(), s.size());
        return to;
    }

}