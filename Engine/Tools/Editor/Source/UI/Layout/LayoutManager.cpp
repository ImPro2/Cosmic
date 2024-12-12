#include "cspch.hpp"
#include "LayoutManager.hpp"

#include "UI/Layout/CustomLayoutModule.hpp"

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

		mSwitchLayout = &layout;
	}

	void LayoutManager::SaveCurrentLayout()
	{
		ModuleSystem::Add<CustomLayoutModule>()->OnNameEntered([this](const String& name)
		{
			mLayouts.emplace_back(name);

			mCurrentLayout = &mLayouts[mCurrentLayoutIndex++];
			mSaveLayout    = true;
		});
	}

	bool LayoutManager::SwitchLayout()
	{
		if (mSwitchLayout == mCurrentLayout)
			mSwitchLayout = nullptr;

		return mSwitchLayout != nullptr;
	}

	Layout& LayoutManager::GetSwitchLayout()
	{
		mCurrentLayout->mIsLoaded = false;

		Layout& layout   = *mSwitchLayout;
		layout.mIsLoaded = true;

		mCurrentLayout = mSwitchLayout;
		mSwitchLayout  = nullptr;

		return layout;
	}

	bool LayoutManager::SaveLayout()
	{
		if (mSaveLayout)
		{
			mSaveLayout = false;
			return true;
		}

		return false;
	}

}
