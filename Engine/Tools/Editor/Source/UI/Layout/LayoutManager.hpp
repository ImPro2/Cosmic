#pragma once
#include "UI/Layout/Layout.hpp"

namespace Cosmic
{

	class LayoutManager
	{
	public:
		void Init();
		void Shutdown();

	public:
		void SwitchLayout(Layout& layout);
		void SaveCurrentLayout(const String& name);

	public:
		Layout&       GetCurrentLayout()       { return *mCurrentLayout; }
		const Layout& GetCurrentLayout() const { return *mCurrentLayout; }

		Layout&       GetDefaultLayout()       { return mLayouts[0];     }
		const Layout& GetDefaultLayout() const { return mLayouts[0];     }

		const Vector<Layout>& GetLayouts() const { return mLayouts; }

	private:
		Vector<Layout> mLayouts;
		Layout*        mCurrentLayout;
	};

}
