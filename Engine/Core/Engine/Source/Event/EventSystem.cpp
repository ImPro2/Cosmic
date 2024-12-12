#include "cspch.hpp"
#include "EventSystem.hpp"

CS_MODULE_LOG_INFO(Cosmic, App.Events);
#include "Base/Base.hpp"
#include "App/App.hpp"

namespace Cosmic
{

	void EventSystem::Init()
	{

	}

	void EventSystem::Shutdown()
	{

	}

	void EventSystem::DispatchEvents()
	{
		while (!sEventQueue.empty())
		{
			FramePtr<IEvent> e = sEventQueue.front();
			Application::Get()->OnEvent(*e);
			sEventQueue.pop();
		}
	}

}
