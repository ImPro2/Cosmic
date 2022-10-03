module;
#include "cspch.hpp"
#include <functional>
export module Cosmic.App.Events;

namespace Cosmic
{

    export enum class EEventType
    {
        None = -1,
        AppInit, AppUpdate, AppClose,
        WindowCreate, WindowClose, WindowResize, WindowMove, WindowTitle,
        KeyPress, KeyRelease, KeyType,
        MouseMove, MouseScroll, MouseButtonClick, MouseButtonRelease
    };

    export struct Event
    {
        virtual EEventType GetType() const = 0;
    };

    export class EventDispatcher
    {
    public:
        EventDispatcher(const Event& event)
            : m_Event(const_cast<Event&>(event))
        {
        }
        
        // F will be deduced by the compiler
        template<typename T, typename F>
        void Dispatch(const F& func)
        {
            CS_PROFILE_FN();

            if (m_Event.GetType() == T::GetStaticType())
            {
                func(static_cast<T&>(m_Event));
            }
        }
    private:
        Event& m_Event;
    };

}