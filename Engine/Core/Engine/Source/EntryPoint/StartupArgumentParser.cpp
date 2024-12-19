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

		bool success = true;

		for (int32 i = 1; i < mStartupArgs.ArgumentCount;)
		{
			success = ParseStartupArgument(i);

			if (!success)
				break;
		}

		if (!success)
		{
			PrintHelp();
			mSpecification.ErrorCallback();
		}
	}

	void StartupArgumentParser::CallRegisteredStartupArguments()
	{
		for (const auto& [callback, value] : mRegisteredCallbackList)
		{
			callback(value);
		}
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

			if (arg.DefaultValue != "")
				line += std::format("={}", arg.DefaultValue.c_str());
			else if (arg.ArgumentType == EStartupArgumentType::Option)
				line += std::format("={}", arg.Name.c_str());

			size_t spaceCount = indentCount - line.size();

			for (size_t i = 0; i < spaceCount; i++)
				line += ' ';
			line += arg.Description + '\n';

			OS::Print(line.c_str());

			for (const String& possibleValue : arg.PossibleValues)
			{
				line = std::format("    {}\n", possibleValue.c_str());
				OS::Print(line.c_str());
			}
		}

		OS::Print(std::format("\n{}\n\n", mSpecification.ProgramBrief.c_str()).c_str());
	}

	bool StartupArgumentParser::ParseStartupArgument(int32& argcIndex)
	{
		String argv                      = String(mStartupArgs.Arguments[argcIndex++]);
		bool   matchedSpecArg            = false;
		int32  matchedPositionalArgCount = 0;
		int32  totalPositionalArgCount   = 0;

		for (StartupArgument& specArg : mSpecification.StartupArguments)
		{
			if (specArg.ArgumentType == EStartupArgumentType::Positional)
				totalPositionalArgCount++;

			if (argv.contains(specArg.Flag) ||
				argv.contains(specArg.AltFlag))
			{
				matchedSpecArg = true;

				switch (specArg.ArgumentType)
				{
					case EStartupArgumentType::Flag:
					{
						if (argv.size() < 2)
						{
							ErrorInvalidFlagFormat(argv);
							return false;
						}

						if ((argv[0] != '-' && argv[1] != '-') || 
							(argv[0] != '-'))
						{
							ErrorInvalidFlagFormat(argv);
							return false;
						}

						mRegisteredCallbackList.push_back({ specArg.Callback, argv });
						break;
					}
					case EStartupArgumentType::Positional:
					{
						matchedPositionalArgCount++;
						mRegisteredCallbackList.push_back({ specArg.Callback, argv });
						break;
					}
					case EStartupArgumentType::Option:
					{
						String value = "";

						if (argv.size() < 2)
						{
							ErrorInvalidOptionFormat(argv);
							return false;
						}

						if (argv[0] == '-' && argv[1] == '-')
						{
							Vector<String> splitArgv = StringUtils::Split(argv, '=');
							value = splitArgv[splitArgv.size() - 1];
						}
						else if (argv[0] == '-')
						{
							if (argcIndex == mStartupArgs.ArgumentCount)
							{
								ErrorInvalidOptionFormat(argv);
								return false;
							}

							value = String(mStartupArgs.Arguments[argcIndex++]);
						}
						else
						{
							ErrorInvalidOptionFormat(argv);
							return false;
						}

						if (specArg.PossibleValues.size() > 0)
						{
							auto it = std::find(specArg.PossibleValues.begin(), specArg.PossibleValues.end(), value);
							if (it == specArg.PossibleValues.end())
							{
								ErrorInvalidOptionValue(value);
								return false;
							}
						}

						mRegisteredCallbackList.push_back({ specArg.Callback, value });
						break;
					}
				}
			}
		}

		if (totalPositionalArgCount > matchedPositionalArgCount)
		{
			ErrorMissingPositionalArgument();
			return false;
		}

		if (!matchedSpecArg) // Incorrect usage
		{
			ErrorInvalidArgument(argv);
			return false;
		}

		return true;
	}

	void StartupArgumentParser::ErrorInvalidFlagFormat(const String& flag)
	{
		OS::Print(std::format("Error: invalid flag format {}\n\n", flag.c_str()).c_str());
	}

	void StartupArgumentParser::ErrorInvalidOptionFormat(const String& option)
	{
		OS::Print(std::format("Error: invalid option format {}\n\n", option.c_str()).c_str());
	}

	void StartupArgumentParser::ErrorInvalidOptionValue(const String& value)
	{
		OS::Print(std::format("Error: invalid option value {}\n\n", value.c_str()).c_str());
	}

	void StartupArgumentParser::ErrorMissingPositionalArgument()
	{
		OS::Print("Error: missing positional argument(s)\n\n");
	}

	void StartupArgumentParser::ErrorInvalidArgument(const String& argument)
	{
		OS::Print(std::format("Error: Invalid argument {}\n\n", argument.c_str()).c_str());
	}

}