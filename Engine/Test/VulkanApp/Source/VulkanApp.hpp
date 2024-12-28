#pragma once
#include "App/Application.hpp"
#include "Event/Events.hpp"
#include "Event/Type/AppEvents.hpp"
#include "EntryPoint/EntryPoint.hpp"

#include "IVulkanDesktopWindow.hpp"

namespace Cosmic
{

	class VulkanApp : public Application
	{
	public:
        VulkanApp(const StartupArgumentList& args);

		void OnEvent(const IEvent& e) override;

		bool OnInit(const ApplicationInitEvent& e);
		bool OnClose(const ApplicationCloseEvent& e);
		bool OnUpdate(const ApplicationUpdateEvent& e);

	private:
		bool mRunning = true;

		Scope<IVulkanDesktopWindow> mWindow;
	};

}
