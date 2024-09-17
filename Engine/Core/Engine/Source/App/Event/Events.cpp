#include "cspch.hpp"

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

	void EventSystem::AddEvent(Event* e)
	{
		sEventQueue.push(e);
	}

	void EventSystem::DispatchEvents()
	{
		while (!sEventQueue.empty())
		{
			Event* e = sEventQueue.front();
			Application::Get()->OnEvent(*e);
			sEventQueue.pop();
		}
	}

}
