#include "cspch.hpp"
#include "VulkanApp.hpp"

#include "IVulkanDesktopWindow.hpp"

namespace Cosmic
{

	VulkanApp::VulkanApp(const StartupArgumentList& args)
	{
		Init();
	}

	void VulkanApp::Init()
	{
		Allocations::Init();

		mWindow = CreateScope<IVulkanDesktopWindow>(DesktopWindowInfo());
		mWindow.As<IVulkanDesktopWindow>()->SetCloseCallback([this]()
		{
			mRunning = false;
		});

		Run();
		Shutdown();
	}

	void VulkanApp::Shutdown()
	{
		Allocations::Shutdown();
	}

	void VulkanApp::Run()
	{
		while (mRunning)
		{
			mWindow->Update();
		}
	}

	Application* CreateApplication(StartupArgumentList&& args)
	{
		return new VulkanApp(std::move(args));
	}

}
