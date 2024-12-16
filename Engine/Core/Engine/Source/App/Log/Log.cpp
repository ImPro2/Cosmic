#include "cspch.hpp"
#include "Log.hpp"
#include "App/Application.hpp"

namespace Cosmic
{

	PersistentRef<Log> Log::Init()
	{
		sInstance = CreatePersistentRef<Log>();
		return sInstance;
	}

	void Log::Shutdown()
	{
		sInstance.Release();
	}

	void Log::LogOut(const char* format, ELogSeverity severity, std::format_args args)
	{
		CS_PROFILE_FN();

		LogData logData = { std::format("{}\n", std::vformat(format, args)), severity };
		sInstance->mLogData.push_back(logData);

		LogToConsole(logData);
		LogToFile(logData);

		EventSystem::DeferEvent<LogEvent>(logData.Message, logData.Severity);
	}

	void Log::LogToConsole(const LogData& logData)
	{
		CS_PROFILE_FN();

		OS::SetConsoleColor(ELogSeverityToConsoleColor(logData.Severity));
		OS::Print(logData.Message.c_str());
		//OS::FlushConsoleLine();
	}

	void Log::LogToFile(const LogData& logData)
	{
		// TODO: Implement
	}

}
