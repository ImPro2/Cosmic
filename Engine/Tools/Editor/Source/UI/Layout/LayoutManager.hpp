#pragma once
#include "UI/Layout/Layout.hpp"

namespace Cosmic
{

	class DockspaceModule;

	class LayoutManager
	{
	public:
		void Init();
		void Shutdown();

	public:
		void SwitchLayout(Layout& layout);
		void SaveCurrentLayout();

	public:
		Layout&       GetCurrentLayout()       { return *mCurrentLayout; }
		const Layout& GetCurrentLayout() const { return *mCurrentLayout; }

		Layout&       GetDefaultLayout()       { return mLayouts[0];     }
		const Layout& GetDefaultLayout() const { return mLayouts[0];     }

		Vector<Layout>&       GetLayouts()       { return mLayouts; }
		const Vector<Layout>& GetLayouts() const { return mLayouts; }

	private:
		bool SwitchLayout();
		Layout& GetSwitchLayout();

		bool SaveLayout();

	private:
		void UpdateMenubar();

	private:
		Vector<Layout> mLayouts;
		Layout*        mCurrentLayout;
		Layout*        mSwitchLayout;

		int32          mCurrentLayoutIndex = 0;

		bool mSaveLayout = false;

		friend class DockspaceModule;
	};

}
