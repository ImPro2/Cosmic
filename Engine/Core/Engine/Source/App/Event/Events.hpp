#pragma once
#include "Base/Base.hpp"
#include "Base/Types.hpp"

#include <functional>
#include <queue>

#define CS_EVENT_TYPE(type)                                       \
           int16 GetType() const override { return (int16)type; } \
    static int16 GetStaticType()          { return (int16)type; }

namespace Cosmic
{

    enum class EEventType : int16
    {
        None = -1,
        LogEvent,
        AppInit, AppUpdate, AppClose,
        WindowCreate, WindowClose, WindowResize, WindowMove, WindowTitle,
        KeyPress, KeyRelease, KeyType,
        MouseMove, MouseScroll, MouseButtonClick, MouseButtonRelease,
        DirectoryAdded, DirectoryRemoved, DirectoryRenamed, DirectoryModified,
        FileAdded, FileRemoved, FileRenamed, FileModified,
        Last
    };

    struct Event
    {
        virtual int16 GetType() const = 0;
        bool Block = false;
    };

    class EventSystem
    {
    public:
        static void Init();
        static void Shutdown();

        static void AddEvent(Event* e);
        
        //template<typename T, typename... Args>
        //static void AddEvent(Args&&... args)
        //{
        //    T* e = new T(std::forward<Args>(args)...);
        //    sEventQueue.push(e);
        //}

    private:
        static void DispatchEvents();

    private:
        inline static std::queue<Event*> sEventQueue;
        friend class Application;
    };

    class EventDispatcher
    {
    public:
        EventDispatcher(const Event& event)
            : mEvent(const_cast<Event&>(event))
        {
        }
        
        // F will be deduced by the compiler
        template<typename T, typename F>
        void Dispatch(const F& func)
        {
            CS_PROFILE_FN();

            if (mEvent.GetType() == T::GetStaticType())
            {
               mEvent.Block |= func(static_cast<T&>(mEvent));
            }
        }
    private:
        Event& mEvent;
    };

}
