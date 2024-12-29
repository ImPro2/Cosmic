#pragma once

#include <format>
#include <string_view>

#include "Base/Base.hpp"
#include "App/OS.hpp"

#include "App/Log/ConsoleColor.hpp"
#include "App/Log/LogSeverity.hpp"

#include "Event/Type/LogEvents.hpp"

#include "Memory/Memory.hpp"

namespace Cosmic
{

	struct LogData
	{
		String Message;
		ELogSeverity Severity;
	};

	class Application;

	class Log : public IRefCounted
	{
	private:
		static PersistentRef<Log> Init();
		static void               Shutdown();

	public:
		static const Vector<LogData>& GetLogData() { return sInstance->mLogData; }

	public:
		template<typename... Args>
		static void LogWithSeverity(const char* format, ELogSeverity severity, const Args&... args)
		{
			LogOut(format, severity, std::make_format_args(args...));
		}

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
		static void LogOut(const char* format, ELogSeverity severity, std::format_args args);
		static void LogToConsole(const LogData& logData);
		static void LogToFile(const LogData& logData);

	private:
		Vector<LogData> mLogData;
		inline static PersistentRef<Log> sInstance;

		friend class Application;
	};

}

#include "LogMacros.hpp"