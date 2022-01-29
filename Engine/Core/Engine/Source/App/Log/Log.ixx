module;
#include "cspch.hpp"
#include <format>
#include <string_view>

export module Cosmic.App.Log;

import Cosmic.Base;
import Cosmic.App.ConsoleColor;
import Cosmic.App.OS;

namespace Cosmic
{

    export enum class ELogSeverity
    {
        Trace,
        Debug,
        Info,
        Warn,
        Error,
        Critical
    };

    export class Log
    {
    public:
        template<typename ... Args>
        static void Trace(std::string_view format, const Args& ... args)
        {
            LogOut(format, ELogSeverity::Trace, std::make_format_args(args...));
        }

        template<typename ... Args>
        static void Debug(const char* format, const Args& ... args)
        {
            LogOut(format, ELogSeverity::Debug, std::make_format_args(args...));
        }
        
        template<typename ... Args>
        static void Info(const char* format, const Args& ... args)
        {
            LogOut(format, ELogSeverity::Info, std::make_format_args(args...));
        }
        
        template<typename ... Args>
        static void Warn(const char* format, const Args& ... args)
        {
            LogOut(format, ELogSeverity::Warn, std::make_format_args(args...));
        }
        
        template<typename ... Args>
        static void Error(const char* format, const Args& ... args)
        {
            LogOut(format, ELogSeverity::Error, std::make_format_args(args...));
        }
        
        template<typename ... Args>
        static void Critical(const char* format, const Args& ... args)
        {
            LogOut(format, ELogSeverity::Critical, std::make_format_args(args...));
        }

    private:
        template<typename ... Args>
        static void LogOut(std::string_view format, ELogSeverity severity, std::format_args args)
        {
            std::string msg = std::vformat(format, args);

            OS::SetConsoleColor(ELogSeverityToConsoleColor(severity));
            OS::Print(msg.c_str());
            // xOS::Print("\n");
            OS::FlushConsoleLine();
        }

        static ConsoleColorPair ELogSeverityToConsoleColor(ELogSeverity severity)
        {
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
    };

}