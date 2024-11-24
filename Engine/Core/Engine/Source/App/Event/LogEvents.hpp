#pragma once
#include "App/Event/Events.hpp"
#include "App/Log/LogSeverity.hpp"

namespace Cosmic
{

    struct LogEvent : public IEvent
    {
        LogEvent(const std::string& msg, ELogSeverity severity)
            : mMessage(msg), mSeverity(severity)
        {
        }

        const std::string& GetMessage() const { return mMessage;  }
        ELogSeverity GetSeverity()      const { return mSeverity; }

        CS_EVENT_TYPE(EEventType::LogEvent);

    private:
        std::string mMessage;
        ELogSeverity mSeverity;
    };

}
