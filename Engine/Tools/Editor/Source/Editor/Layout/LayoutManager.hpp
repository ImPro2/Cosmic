#pragma once
#include "Editor/Layout/Layout.hpp"

namespace Cosmic
{

	class LayoutManager
	{
	public:
		void OnImGuiRender();

	public:
		void SetupDefaultLayout(const Layout& mLayout);
		void SaveCurrentLayout(const String& name);

	public:
		const Layout&         GetCurrentLayout() const { return *mCurrentLayout; }
		const Vector<Layout>& GetLayouts()       const { return mLayouts;        }

		const String&         GetDockspaceName() const { return mDockspaceName;  }

	private:
		Vector<Layout> mLayouts;
		Layout*        mCurrentLayout;
		String         mDockspaceName = "Editor Dockspace";
	};

}
