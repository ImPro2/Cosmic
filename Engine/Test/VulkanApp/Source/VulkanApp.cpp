#include "cspch.hpp"
#include "VulkanApp.hpp"

#include "IVulkanDesktopWindow.hpp"

namespace Cosmic
{

	VulkanApp::VulkanApp(const StartupArgumentList& args)
	{
		ApplicationInfo info;
		info.EnabledSystems.Unset({
			EEngineSystem::Renderer,
			EEngineSystem::AppWindow,
			EEngineSystem::GUI,
			EEngineSystem::NativeScriptEngine,
			EEngineSystem::ProjectManager
		});

		Init(info);
	}

	void VulkanApp::OnEvent(const IEvent& e)
	{
		EventDispatcher dispatcher(e);
		CS_DISPATCH_EVENT(ApplicationInitEvent,   OnInit);
		CS_DISPATCH_EVENT(ApplicationCloseEvent,  OnClose);
		CS_DISPATCH_EVENT(ApplicationUpdateEvent, OnUpdate);

		Application::OnEvent(e);
	}

	bool VulkanApp::OnInit(const ApplicationInitEvent& e)
	{
		mWindow = CreateScope<IVulkanDesktopWindow>(DesktopWindowInfo());
		mWindow->SetCloseCallback([this]() { Close(); });

		return false;
	}

	bool VulkanApp::OnClose(const ApplicationCloseEvent& e)
	{
		mWindow->Close();

		return false;
	}

	bool VulkanApp::OnUpdate(const ApplicationUpdateEvent& e)
	{
		mWindow->Update();

		return false;
	}

	Application* CreateApplication(StartupArgumentList&& args)
	{
		return new VulkanApp(std::move(args));
	}

}
