#pragma once
#include "Base/Base.hpp"
#include "EntryPoint/StartupArgumentList.hpp"

namespace Cosmic
{

	enum class EStartupArgumentType
	{
		Positional, // positional argument       e.g. file
		Flag,       // on/off flag               e.g. --flag
		Option      // option that takes a value e.g. -f file.txt or --file=file.txt
	};


	using StartupArgumentCallback      = std::function<void(const String&)>;
	using StartupArgumentErrorCallback = std::function<void()>;

	struct StartupArgument
	{
		StartupArgument() = default;
		StartupArgument(
			EStartupArgumentType            type,
			const char*                     name,
			const char*                     flag           = "",
			const char*                     altFlag        = "",
			const char*                     category       = "",
			const char*                     description    = "",
			const char*                     defaultValue   = "",
			std::initializer_list<String>&& possibleValues = { },
			StartupArgumentCallback         callback       = nullptr
		)
			: ArgumentType(type), Name(name), Flag(flag), AltFlag(altFlag), Category(category), Description(description), DefaultValue(defaultValue), PossibleValues(possibleValues), Callback(callback)
		{
		}

		EStartupArgumentType ArgumentType;

		String Name;
		String Flag, AltFlag;
		String Category;
		String Description;
		String DefaultValue;
		Vector<String> PossibleValues;

		StartupArgumentCallback Callback;
	};

#if 0
	StartupArgument(EStartupArgumentType::Positional, "Starting Scene");
	StartupArgument(EStartupArgumentType::Flag, "Debug", "--debug", "-d", "On");
	StartupArgument(EStartupArgumentType::Option, "Debug", "--debug")
#endif

	struct StartupArgumentSpecification
	{
		StartupArgumentSpecification() = default;
		StartupArgumentSpecification(
			const char*                              programName,
			const char*                              programDescription,
			const char*                              programBrief,
			std::initializer_list<String>&&          startupArgumentCategories,
			std::initializer_list<StartupArgument>&& startupArguments,
			StartupArgumentErrorCallback             errorCallback
		)
			: ProgramName(programName), ProgramDescription(programDescription), ProgramBrief(programBrief), StartupArgumentCategories(startupArgumentCategories), StartupArguments(std::move(startupArguments)), ErrorCallback(errorCallback)
		{
		}

		String ProgramName;
		String ProgramDescription;
		String ProgramBrief;

		Vector<String>          StartupArgumentCategories;
		Vector<StartupArgument> StartupArguments;

		StartupArgumentErrorCallback ErrorCallback;
	};

}
