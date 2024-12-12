#include "cspch.hpp"
#include "LayoutManager.hpp"

CS_MODULE_LOG_INFO(Editor, UI.Layout.LayoutManager);

namespace Cosmic
{

	void LayoutManager::Init()
	{
		mLayouts.reserve(10);
		mLayouts.emplace_back();

		mCurrentLayout = &mLayouts[mCurrentLayoutIndex++];
		mSwitchLayout  = mCurrentLayout;

		mCurrentLayout->ConstructDefaultLayout();
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
		mSwitchLayout  = &layout;
	}

	void LayoutManager::SaveCurrentLayout(const String& name)
	{
		mLayouts.emplace_back();
		mCurrentLayout = &mLayouts[mCurrentLayoutIndex++];

		mCurrentLayout->ConstructFromCurrentLayout();
	}

	Layout& LayoutManager::GetSwitchLayout()
	{
		Layout& layout = *mSwitchLayout;
		mSwitchLayout  = nullptr;
		return layout;
	}

}
