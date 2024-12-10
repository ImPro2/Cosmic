#pragma once

#include "Event/IEvent.hpp"
#include "App/Application.hpp"

namespace Cosmic
{

    struct IApplicationEvent : public IEvent
    {
    };

    struct ApplicationInitEvent : public IApplicationEvent
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

    struct ApplicationUpdateEvent : public IApplicationEvent
    {
        CS_EVENT_TYPE(EEventType::AppUpdate);
    };

    struct ApplicationCloseEvent : public IApplicationEvent
    {
        CS_EVENT_TYPE(EEventType::AppClose);
    };

}
