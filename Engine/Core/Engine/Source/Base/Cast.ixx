module;
#define _CRT_SECURE_NO_WARNINGS
#include "cspch.hpp"
export module Cosmic.Base.Cast;

namespace Cosmic
{

    export template<typename From, typename To>
    To Cast(From from)
    {
        //static_assert(false);
    }

    export std::wstring Cast(std::string s)
    {
        CS_PROFILE_FN();

        std::wstring to = L"";
        mbstowcs((wchar_t*)to.c_str(), s.c_str(), s.size());
        return to;
    }

    export std::string Cast(std::wstring s)
    {
        CS_PROFILE_FN();

        std::string to = "";
        wcstombs((char*)to.c_str(), s.c_str(), s.size());
        return to;
    }

}