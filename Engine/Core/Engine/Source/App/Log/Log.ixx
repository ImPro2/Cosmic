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
        static void Trace(const char* format, const Args& ... args)
        {
            CS_PROFILE_FN();

            LogOut(format, ELogSeverity::Trace, std::make_format_args(args...));
        }

        template<typename ... Args>
        static void Debug(const char* format, const Args& ... args)
        {
            CS_PROFILE_FN();

            LogOut(format, ELogSeverity::Debug, std::make_format_args(args...));
        }
        
        template<typename ... Args>
        static void Info(const char* format, const Args& ... args)
        {
            CS_PROFILE_FN();

            LogOut(format, ELogSeverity::Info, std::make_format_args(args...));
        }
        
        template<typename ... Args>
        static void Warn(const char* format, const Args& ... args)
        {
            CS_PROFILE_FN();

            LogOut(format, ELogSeverity::Warn, std::make_format_args(args...));
        }
        
        template<typename ... Args>
        static void Error(const char* format, const Args& ... args)
        {
            CS_PROFILE_FN();

            LogOut(format, ELogSeverity::Error, std::make_format_args(args...));
        }
        
        template<typename ... Args>
        static void Critical(const char* format, const Args& ... args)
        {
            CS_PROFILE_FN();

            LogOut(format, ELogSeverity::Critical, std::make_format_args(args...));
        }

    private:
        template<typename ... Args>
        static void LogOut(const char* format, ELogSeverity severity, std::format_args args)
        {
            CS_PROFILE_FN();

            std::string msg = std::format("{}\n", std::vformat(format, args));

            LogToConsole(msg, severity);
            LogToFile(msg, severity);
        }

        static void LogToConsole(const String& msg, ELogSeverity severity)
        {
            CS_PROFILE_FN();

            OS::SetConsoleColor(ELogSeverityToConsoleColor(severity));
            OS::Print(msg.c_str());
            //OS::FlushConsoleLine();
        }

        static void LogToFile(const String& msg, ELogSeverity severity)
        {
            // TODO: Implement
        }

        static ConsoleColorPair ELogSeverityToConsoleColor(ELogSeverity severity)
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
    };

}