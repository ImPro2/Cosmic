#include "cspch.hpp"
#include "LayoutManager.hpp"

CS_MODULE_LOG_INFO(Editor, UI.Layout.LayoutManager);

namespace Cosmic
{

	void LayoutManager::Init()
	{
		Layout layout;
		layout.ConstructDefaultLayout();

		mCurrentLayout = &layout;

		mLayouts.reserve(10);
		mLayouts.push_back(layout);
	}


	void LayoutManager::Shutdown()
	{
	}

	void LayoutManager::SwitchLayout(Layout& layout)
	{
		if (std::find(mLayouts.begin(), mLayouts.end(), layout) == mLayouts.end())
		{
			CS_LOG_ERROR("Unregistered layout");
			return;
		}

		mCurrentLayout = &layout;
		layout.Load();
	}

	void LayoutManager::SaveCurrentLayout(const String& name)
	{
		Layout layout;
		layout.ConstructFromCurrentLayout();

		mLayouts.push_back(layout);
		mCurrentLayout = &layout;
	}

}
