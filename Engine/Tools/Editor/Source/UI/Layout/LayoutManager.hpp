#pragma once
#include "UI/Layout/Layout.hpp"
#include "App/Path.hpp"

namespace Cosmic
{

	class DockspaceModule;

	class LayoutManager
	{
	public:
		void Init(const Path& savePath = "");
		void Shutdown();

	public:
		void SwitchLayout(Layout& layout);
		void SaveCurrentLayout();
		void SerializeLayouts();

	public:
		Layout&       GetCurrentLayout()       { return *mCurrentLayout; }
		const Layout& GetCurrentLayout() const { return *mCurrentLayout; }

		Layout&       GetDefaultLayout()       { return mLayouts[0];     }
		const Layout& GetDefaultLayout() const { return mLayouts[0];     }

		Vector<Layout>&       GetLayouts()       { return mLayouts; }
		const Vector<Layout>& GetLayouts() const { return mLayouts; }

		const Path& GetSavePath() const { return mSavePath; }

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

		Path           mSavePath;

		bool mSaveLayout = false;

		friend class DockspaceModule;
	};

}
