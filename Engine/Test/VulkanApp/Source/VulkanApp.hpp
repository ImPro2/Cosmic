#pragma once
#include "App/Application.hpp"
#include "Event/Events.hpp"
#include "Event/Type/AppEvents.hpp"
#include "EntryPoint/EntryPoint.hpp"

namespace Cosmic
{

	class VulkanApp : public Application
	{
	public:
        VulkanApp(const StartupArgumentList& args);

		void OnEvent(const IEvent& e) override;
		bool OnInit(const ApplicationInitEvent& e);
	};

}
