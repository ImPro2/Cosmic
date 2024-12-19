#pragma once
#include "EntryPoint/StartupArgumentList.hpp"
#include "EntryPoint/StartupArgumentSpecification.hpp"

namespace Cosmic
{

	using RegisteredStartupArgumentCallbackList = Vector<Pair<StartupArgumentCallback, String>>; // value - callback

	class StartupArgumentParser
	{
	public:
		StartupArgumentParser() = default;
		StartupArgumentParser(const StartupArgumentList& args);

	public:
		void Parse(const StartupArgumentSpecification& spec);
		void CallRegisteredStartupArguments();

		void PrintHelp();

	private:
		bool ParseStartupArgument(int32& argcIndex);

		void ErrorInvalidFlagFormat(const String& flag);
		void ErrorInvalidOptionFormat(const String& option);
		void ErrorInvalidOptionValue(const String& value);
		void ErrorMissingPositionalArgument();
		void ErrorInvalidArgument(const String& argument);

	private:
		StartupArgumentList          mStartupArgs;
		StartupArgumentSpecification mSpecification;

		RegisteredStartupArgumentCallbackList mRegisteredCallbackList;
	};

}
