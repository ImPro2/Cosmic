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


	using StartupArgumentCallback = std::function<void(const String&)>;

	struct StartupArgument
	{
		StartupArgument() = default;
		StartupArgument(
			EStartupArgumentType    type,
			const char*             name,
			const char*             flag                  = "",
			const char*             altFlag               = "",
			const char*             category              = "",
			const char*             description           = "",
			std::initializer_list<String>&& defaultValues = { },
			StartupArgumentCallback callback              = nullptr
		)
			: ArgumentType(type), Name(name), Flag(flag), AltFlag(altFlag), Category(category), Description(description), DefaultValues(defaultValues), Callback(callback)
		{
		}

		EStartupArgumentType ArgumentType;

		String Name;
		String Flag, AltFlag;
		String Category;
		String Description;
		Vector<String> DefaultValues;

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
			const char* programName,
			const char* programDescription,
			const char* programBrief,
			std::initializer_list<String>&&          startupArgumentCategories,
			std::initializer_list<StartupArgument>&& startupArguments
		)
			: ProgramName(programName), ProgramDescription(programDescription), ProgramBrief(programBrief), StartupArgumentCategories(startupArgumentCategories), StartupArguments(std::move(startupArguments))
		{
		}

		String ProgramName;
		String ProgramDescription;
		String ProgramBrief;

		Vector<String>          StartupArgumentCategories;
		Vector<StartupArgument> StartupArguments;
	};

}
