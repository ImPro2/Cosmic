module;
#include "cspch.hpp"
export module Editor.TabBars;

import Cosmic.Base;

namespace Cosmic
{

	export class ITabBar
	{
	public:
		ITabBar(const char* name)
			: mTabBarName(name)
		{
		}

	protected:
		String mTabBarName = "";
		bool   mModified = false;
	};

}
