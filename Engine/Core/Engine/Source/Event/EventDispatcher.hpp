#pragma once
#include "IEvent.hpp"

namespace Cosmic
{

    class EventDispatcher
    {
    public:
        EventDispatcher(const IEvent& event)
            : mEvent(const_cast<IEvent&>(event))
        {
        }
        
        // F will be deduced by the compiler
        template<typename T, typename F>
        void Dispatch(const F& func)
        {
            CS_PROFILE_FN();

            if ((IEvent*)&mEvent == nullptr)
                return;

            if (mEvent.GetType() == T::GetStaticType())
            {
               mEvent.Block |= func(static_cast<T&>(mEvent));
            }
        }
    private:
        IEvent& mEvent;
    };

}
