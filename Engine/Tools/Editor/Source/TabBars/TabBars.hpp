#pragma once
#include "Base/Base.hpp"

namespace Cosmic
{

	class ITabBar
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
