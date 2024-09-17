#pragma once

#include "App/Log/ConsoleColor.hpp"

namespace Cosmic
{

    enum class ELogSeverity
    {
        Trace,
        Debug,
        Info,
        Warn,
        Error,
        Critical
    };

    inline ConsoleColorPair ELogSeverityToConsoleColor(ELogSeverity severity)
    {
        CS_PROFILE_FN();

        switch (severity)
        {
            case ELogSeverity::Trace:    return { EConsoleColor::BrightWhite,  EConsoleColor::Black };
            case ELogSeverity::Debug:    return { EConsoleColor::BrightCyan,   EConsoleColor::Black };
            case ELogSeverity::Info:     return { EConsoleColor::BrightGreen,  EConsoleColor::Black };
            case ELogSeverity::Warn:     return { EConsoleColor::BrightYellow, EConsoleColor::Black };
            case ELogSeverity::Error:    return { EConsoleColor::BrightRed,    EConsoleColor::Black };
            case ELogSeverity::Critical: return { EConsoleColor::Red,          EConsoleColor::White };
        }
    }

}