#include "cspch.hpp"
#include "StartupArgumentParser.hpp"

namespace Cosmic
{

	StartupArgumentParser::StartupArgumentParser(const StartupArgumentList& args)
		: mStartupArgs(args)
	{
	}

	void StartupArgumentParser::Parse(const StartupArgumentSpecification& spec)
	{
		mSpecification = spec;
	}

	void StartupArgumentParser::PrintHelp()
	{
		OS::SetConsoleColor({ EConsoleColor::White, EConsoleColor::Black });
		OS::Print(std::format("{}\n{}\n\n", mSpecification.ProgramName, mSpecification.ProgramDescription).c_str());

		String usageString = std::format("Usage: {} ", mSpecification.ProgramName.c_str());
		String lastCategory = "";

		for (const StartupArgument& arg : mSpecification.StartupArguments)
		{
			if (arg.ArgumentType == EStartupArgumentType::Positional)
			{
				usageString += std::format("{} ", arg.Name);
			}
			else if (lastCategory != arg.Category)
			{
				lastCategory = arg.Category;
				usageString += std::format("[{}] ", lastCategory.c_str());
			}
		}

		usageString += "\n";

		OS::Print(usageString.c_str());

		lastCategory = "";
		const size_t indentCount = 30;

		for (const StartupArgument& arg : mSpecification.StartupArguments)
		{
			if (lastCategory != arg.Category)
			{
				lastCategory = arg.Category;
				OS::Print(std::format("\n======================\n  {}\n======================\n\n", lastCategory.c_str()).c_str());
			}

			String line = arg.AltFlag == "" ? arg.Flag : std::format("{}, {}", arg.Flag, arg.AltFlag);

			if (arg.DefaultValues.size() == 1)
				line += std::format("={}", arg.DefaultValues[0].c_str());
			else if (arg.ArgumentType == EStartupArgumentType::Option)
				line += std::format("={}", arg.Name.c_str());

			size_t spaceCount = indentCount - line.size();

			for (size_t i = 0; i < spaceCount; i++)
				line += ' ';
			line += arg.Description + '\n';

			OS::Print(line.c_str());

			if (arg.DefaultValues.size() <= 1)
				continue;

			for (const String& defaultValue : arg.DefaultValues)
			{
				line = std::format("    {}\n", defaultValue.c_str());
				OS::Print(line.c_str());
			}
		}

		OS::Print(std::format("\n{}\n\n", mSpecification.ProgramBrief.c_str()).c_str());
	}

}