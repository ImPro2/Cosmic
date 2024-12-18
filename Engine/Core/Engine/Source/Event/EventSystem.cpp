#include "cspch.hpp"
#include "EventSystem.hpp"

CS_MODULE_LOG_INFO(Cosmic, App.Events);
#include "Base/Base.hpp"
#include "App/App.hpp"

namespace Cosmic
{

	PersistentRef<EventSystem> EventSystem::Init()
	{
		sInstance = CreatePersistentRef<EventSystem>();

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

			if (e)
				Application::Get()->OnEvent(*e);

			sInstance->mEventQueue.pop();
		}
	}

}
