#pragma once
#include "EntryPoint/StartupArgumentList.hpp"
#include "EntryPoint/StartupArgumentSpecification.hpp"

namespace Cosmic
{

	class StartupArgumentParser
	{
	public:
		StartupArgumentParser() = default;
		StartupArgumentParser(const StartupArgumentList& args);

	public:
		void Parse(const StartupArgumentSpecification& spec);
		void PrintHelp();

	private:
		StartupArgumentList          mStartupArgs;
		StartupArgumentSpecification mSpecification;
	};

}
