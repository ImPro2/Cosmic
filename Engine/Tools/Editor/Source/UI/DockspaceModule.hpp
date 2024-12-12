#pragma once
#include "App/Module.hpp"

namespace Cosmic
{

	class DockspaceModule : public IModule
	{
	public:
		void OnInit()        override;
		void OnShutdown()    override;
		void OnImGuiRender() override;

	public:
		const String& GetDockspaceName() const { return sDockspaceName; }

	private:
		void LoadLayout();

	private:
		inline static const String sDockspaceName = "Editor Dockspace";
	};

}
