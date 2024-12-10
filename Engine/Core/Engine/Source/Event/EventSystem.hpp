#pragma once
#include "IEvent.hpp"
#include "App/FrameAllocator.hpp"

#include <queue>

namespace Cosmic
{

    class EventSystem
    {
    public:
        template<typename T, typename... Args>
        static void DeferEvent(Args&&... args)
        {
            sEventQueue.push(FrameAllocator::Allocate<T>(std::forward<Args>(args)...));
        }
        
    private:
        static void Init();
        static void Shutdown();
        static void DispatchEvents();

    private:
        inline static std::queue<IEvent*> sEventQueue;
        friend class Application;
    };

}
