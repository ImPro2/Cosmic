#pragma once
#include "Base/Types.hpp"

#define CS_EVENT_TYPE(type)                                       \
           int16 GetType() const override { return (int16)type; } \
    static int16 GetStaticType()          { return (int16)type; }

namespace Cosmic
{

    enum class EEventType : int16
    {
        None = -1,
        LogEvent,
        AppInit, AppUpdate, AppClose,
        WindowCreate, WindowClose, WindowResize, WindowMove, WindowTitle,
        KeyPress, KeyRelease, KeyType,
        MouseMove, MouseScroll, MouseButtonClick, MouseButtonRelease,
        DirectoryAdded, DirectoryRemoved, DirectoryRenamed, DirectoryModified,
        FileAdded, FileRemoved, FileRenamed, FileModified,
        Last
    };

    struct IEvent
    {
        virtual int16 GetType() const = 0;
        bool Block = false;
    };

}
