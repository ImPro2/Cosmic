#pragma once
#include "App/Application.hpp"
#include "Event/Events.hpp"
#include "Event/Type/AppEvents.hpp"
#include "EntryPoint/EntryPoint.hpp"

#include "App/Window/IWindow.hpp"

namespace Cosmic
{

	class VulkanApp : public Application
	{
	public:
        VulkanApp(const StartupArgumentList& args);

	private:
		void Init();
		void Shutdown();

		void Run();

	private:
		bool mRunning = true;

		Scope<IDesktopWindow> mWindow;
	};

}
