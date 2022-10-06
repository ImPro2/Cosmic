module;
#include "cspch.hpp"
#include <functional>
export module Cosmic.App.Events;

namespace Cosmic
{

    export enum class EEventType
    {
        None = -1,
        LogEvent,
        AppInit, AppUpdate, AppClose,
        WindowCreate, WindowClose, WindowResize, WindowMove, WindowTitle,
        KeyPress, KeyRelease, KeyType,
        MouseMove, MouseScroll, MouseButtonClick, MouseButtonRelease
    };

    export struct Event
    {
        virtual EEventType GetType() const = 0;
        bool Block = false;
    };

    export class EventDispatcher
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