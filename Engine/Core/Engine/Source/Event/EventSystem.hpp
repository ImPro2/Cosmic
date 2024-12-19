#pragma once
#include "IEvent.hpp"
#include "Memory/Memory.hpp"

#include <queue>

namespace Cosmic
{

    class EventSystem : public IRefCounted
    {
    public:
        template<typename T, typename... Args>
        static void DeferEvent(Args&&... args)
        {
            sInstance->mEventQueue.push(CreateFramePtr<T>(std::forward<Args>(args)...).As<IEvent>());
        }
        
    private:
        static PersistentRef<EventSystem> Init();
        static void                       Shutdown();

        static void DispatchEvents();

    private:
        std::queue<FramePtr<IEvent>> mEventQueue;

        inline static PersistentRef<EventSystem> sInstance;

        friend class Application;
    };

}
