#pragma once

#include <functional>
#include <queue>

namespace Cosmic
{

    enum class EEventType
    {
        None = -1,
        LogEvent,
        AppInit, AppUpdate, AppClose,
        WindowCreate, WindowClose, WindowResize, WindowMove, WindowTitle,
        KeyPress, KeyRelease, KeyType,
        MouseMove, MouseScroll, MouseButtonClick, MouseButtonRelease,
        DirectoryAdded, DirectoryRemoved, DirectoryRenamed, DirectoryModified,
        FileAdded, FileRemoved, FileRenamed, FileModified,
        EditorSceneSaved, EditorSceneSavedAs, EditorSceneOpened, EditorSceneNew
    };

    struct Event
    {
        virtual EEventType GetType() const = 0;
        bool Block = false;
    };

    class EventSystem
    {
    public:
        static void Init();
        static void Shutdown();

        static void AddEvent(Event* e);

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
