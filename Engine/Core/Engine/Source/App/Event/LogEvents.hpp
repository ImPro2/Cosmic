#pragma once
#include "App/Event/Events.hpp"
#include "App/Log/LogSeverity.hpp"

namespace Cosmic
{

    struct LogEvent : public Event
    {
    public:
        LogEvent(const std::string& msg, ELogSeverity severity)
            : mMessage(msg), mSeverity(severity)
        {
        }

    public:
        const std::string& GetMessage() const { return mMessage;  }
        ELogSeverity GetSeverity()      const { return mSeverity; }

    public:
               EEventType GetType() const { return EEventType::LogEvent; }
        static EEventType GetStaticType() { return EEventType::LogEvent; }

    private:
        std::string mMessage;
        ELogSeverity mSeverity;
    };

}