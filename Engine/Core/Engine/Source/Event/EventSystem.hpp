#pragma once
#include "IEvent.hpp"
#include "Memory/FramePtr.hpp"

#include <queue>

namespace Cosmic
{

    class EventSystem
    {
    public:
        template<typename T, typename... Args>
        static void DeferEvent(Args&&... args)
        {
            sEventQueue.push(CreateFramePtr<T>(std::forward<Args>(args)...).As<IEvent>());
        }
        
    private:
        static void Init();
        static void Shutdown();
        static void DispatchEvents();

    private:
        inline static std::queue<FramePtr<IEvent>> sEventQueue;
        friend class Application;
    };

}
