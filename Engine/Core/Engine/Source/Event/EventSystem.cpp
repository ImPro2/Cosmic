#include "cspch.hpp"
#include "EventSystem.hpp"

CS_MODULE_LOG_INFO(Cosmic, App.Events);
#include "Base/Base.hpp"
#include "App/App.hpp"

namespace Cosmic
{

	Ref<EventSystem> EventSystem::Init()
	{
		sInstance = CreateRef<EventSystem>();

		return sInstance;
	}

	void EventSystem::Shutdown()
	{
		sInstance.Release();
	}

	void EventSystem::DispatchEvents()
	{
		while (!sInstance->mEventQueue.empty())
		{
			FramePtr<IEvent> e = sInstance->mEventQueue.front();
			Application::Get()->OnEvent(*e);
			sInstance->mEventQueue.pop();
		}
	}

}
