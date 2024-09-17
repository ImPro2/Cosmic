#pragma once

#include "App/Event/Events.hpp"
#include "App/Application.hpp"

namespace Cosmic
{

    struct ApplicationEvent : public Event
    {
    };

    struct ApplicationInitEvent : public ApplicationEvent
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

    struct ApplicationUpdateEvent : public ApplicationEvent
    {
    public:
               EEventType GetType() const { return EEventType::AppUpdate; }
        static EEventType GetStaticType() { return EEventType::AppUpdate; }
    };

    struct ApplicationCloseEvent : public ApplicationEvent
    {
    public:
               EEventType GetType() const { return EEventType::AppClose; }
        static EEventType GetStaticType() { return EEventType::AppClose; }
    };

}