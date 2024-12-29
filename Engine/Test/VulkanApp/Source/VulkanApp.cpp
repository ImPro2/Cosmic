#include "cspch.hpp"
#include "VulkanApp.hpp"

#include "VulkanModule.hpp"

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

		Application::OnEvent(e);
	}

	bool VulkanApp::OnInit(const ApplicationInitEvent& e)
	{
		ModuleSystem::Add<VulkanModule>();
		return false;
	}

	Application* CreateApplication(StartupArgumentList&& args)
	{
		return new VulkanApp(std::move(args));
	}

}
