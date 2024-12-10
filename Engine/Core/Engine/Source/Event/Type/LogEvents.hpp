#pragma once
#include "Event/IEvent.hpp"
#include "App/Log/LogSeverity.hpp"

namespace Cosmic
{

    struct LogEvent : public IEvent
    {
        LogEvent(const String& msg, ELogSeverity severity)
            : mMessage(msg), mSeverity(severity)
        {
        }

        const String& GetMessage()  const { return mMessage;  }
        ELogSeverity  GetSeverity() const { return mSeverity; }

        CS_EVENT_TYPE(EEventType::LogEvent);

    private:
        String       mMessage;
        ELogSeverity mSeverity;
    };

}
