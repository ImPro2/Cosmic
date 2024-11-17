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
        const ApplicationInfo& GetInfo() const { return mInfo; }

        CS_EVENT_TYPE(EEventType::AppInit);

    private:
        ApplicationInfo mInfo;
    };

    struct ApplicationUpdateEvent : public ApplicationEvent
    {
        CS_EVENT_TYPE(EEventType::AppUpdate);
    };

    struct ApplicationCloseEvent : public ApplicationEvent
    {
        CS_EVENT_TYPE(EEventType::AppClose);
    };

}
