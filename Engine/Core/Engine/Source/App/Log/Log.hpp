#pragma once

#include <format>
#include <string_view>

#include "Base/Base.hpp"
#include "App/Log/ConsoleColor.hpp"
#include "App/OS.hpp"
#include "App/Application.hpp"
#include "App/Event/LogEvents.hpp"
#include "App/Event/Events.hpp"
#include "App/Log/LogSeverity.hpp"

namespace Cosmic
{

    struct LogData
    {
        String Message;
        ELogSeverity Severity;
    };

    class Log
    {
	public:
		static const Vector<LogData>& GetLogData() { return sLogData; }

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

            LogData logData = { std::format("{}\n", std::vformat(format, args)), severity };
            sLogData.push_back(logData);

            LogToConsole(logData);
            LogToFile(logData);

            EventSystem::AddEvent(new LogEvent(logData.Message, logData.Severity));
        }

        static void LogToConsole(const LogData& logData)
        {
            CS_PROFILE_FN();

            OS::SetConsoleColor(ELogSeverityToConsoleColor(logData.Severity));
            OS::Print(logData.Message.c_str());
            //OS::FlushConsoleLine();
        }

        static void LogToFile(const LogData& logData)
        {
            // TODO: Implement
        }

    private:
        inline static Vector<LogData> sLogData;
    };

}

#include "LogMacros.hpp"