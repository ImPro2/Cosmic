module;
#include "cspch.hpp"
export module Cosmic.App.AppEvents;

import Cosmic.App.Events;
import Cosmic.App.Application;

namespace Cosmic
{

    export struct ApplicationEvent : public Event
    {
    };

    export struct ApplicationInitEvent : public ApplicationEvent
    {
    public:
        ApplicationInitEvent(const ApplicationInfo& info)
            : mInfo(info)
        {
        }

    public:
        const  ApplicationInfo& GetInfo() const { return mInfo;               }
               EEventType       GetType() const { return EEventType::AppInit; }
        static EEventType       GetStaticType() { return EEventType::AppInit; }

    private:
        ApplicationInfo mInfo;
    };

    export struct ApplicationUpdateEvent : public ApplicationEvent
    {
    public:
               EEventType GetType() const { return EEventType::AppUpdate; }
        static EEventType GetStaticType() { return EEventType::AppUpdate; }
    };

    export struct ApplicationCloseEvent : public ApplicationEvent
    {
    public:
               EEventType GetType() const { return EEventType::AppClose; }
        static EEventType GetStaticType() { return EEventType::AppClose; }
    };

}