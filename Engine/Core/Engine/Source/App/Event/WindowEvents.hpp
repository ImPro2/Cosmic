#pragma once

#include <functional>

#include "App/Event/Events.hpp"
#include "Base/Base.hpp"
#include "Base/Types.hpp"
#include "Base/Tuples.hpp"
#include "App/KeyAndMouseCodes.hpp"
#include "App/Window/WindowInfo.hpp"

namespace Cosmic
{

    struct WindowEvent : public Event
    {
    public:
        WindowEvent(const DesktopWindowInfo& info, bool isPrimary)
            : mInfo(info), mIsPrimary(isPrimary)
        {
        }

    public:
        inline bool                     IsPrimary() const { return mIsPrimary; }
        inline const DesktopWindowInfo& GetInfo()   const { return mInfo;      }

    private:
        bool                     mIsPrimary;
        const DesktopWindowInfo& mInfo;
    };

    struct WindowCreateEvent : public WindowEvent
    {
        WindowCreateEvent(const DesktopWindowInfo& info, bool isPrimary)
            : WindowEvent(info, isPrimary)
        {
        }

        CS_EVENT_TYPE(EEventType::WindowCreate);
    };

    struct WindowCloseEvent : public WindowEvent
    {
        WindowCloseEvent(const DesktopWindowInfo& info, bool isPrimary)
            : WindowEvent(info, isPrimary)
        {
        }

        CS_EVENT_TYPE(EEventType::WindowClose);
    };

    struct WindowResizeEvent : public WindowEvent
    {
        WindowResizeEvent(uint2 size, const DesktopWindowInfo& info, bool isPrimary)
            : mSize(size), WindowEvent(info, isPrimary)
        {
        }

        inline uint2  GetSize()   const { return mSize;        }
        inline uint32 GetWidth()  const { return mSize.width;  }
        inline uint32 GetHeight() const { return mSize.height; }

        CS_EVENT_TYPE(EEventType::WindowResize);

    private:
        uint2 mSize;
    };

    struct WindowMoveEvent : public WindowEvent
    {
        WindowMoveEvent(int2 pos, const DesktopWindowInfo& info, bool isPrimary)
            : mPos(pos), WindowEvent(info, isPrimary)
        {
        }

        inline int2  GetPosition()  const { return mPos;   }
        inline int32 GetXPosition() const { return mPos.x; }
        inline int32 GetYPosition() const { return mPos.y; }

        CS_EVENT_TYPE(EEventType::WindowMove);

    private:
       int2 mPos;
    };

    struct WindowTitleEvent : public WindowEvent
    {
        WindowTitleEvent(const String& title, const DesktopWindowInfo& info, bool isPrimary)
            : mTitle(title), WindowEvent(info, isPrimary)
        {
        }

        inline const String& GetTitle()  const { return mTitle; }

        CS_EVENT_TYPE(EEventType::WindowTitle);

    private:
        String mTitle;
    };

    struct KeyPressEvent : public WindowEvent
    {
        KeyPressEvent(EKeyCode key, uint8 repeatCount, const DesktopWindowInfo& info, bool isPrimary)
            : mKey(key), mRepeatCount(repeatCount), WindowEvent(info, isPrimary)
        {
        }

        inline EKeyCode GetKeyCode()     const { return mKey;         }
        inline uint8    GetRepeatCount() const { return mRepeatCount; }

        CS_EVENT_TYPE(EEventType::KeyPress);

    private:
        EKeyCode mKey;
        uint8    mRepeatCount;
    };

    struct KeyReleaseEvent : public WindowEvent
    {
        KeyReleaseEvent(EKeyCode key, const DesktopWindowInfo& info, bool isPrimary)
            : mKey(key), WindowEvent(info, isPrimary)
        {
        }

        inline EKeyCode GetKeyCode() const { return mKey; }

        CS_EVENT_TYPE(EEventType::KeyRelease);

    private:
        EKeyCode mKey;
    };

    struct KeyTypeEvent : public WindowEvent
    {
        KeyTypeEvent(char character, const DesktopWindowInfo& info, bool isPrimary)
            : mChar(character), WindowEvent(info, isPrimary)
        {
        }

        inline char GetChar() const { return mChar; }

        CS_EVENT_TYPE(EEventType::KeyType);

    private:
        char mChar;
    };

    struct MouseMoveEvent : public WindowEvent
    {
        MouseMoveEvent(float2 pos, const DesktopWindowInfo& info, bool isPrimary)
            : mPos(pos), WindowEvent(info, isPrimary)
        {
        }

        inline float2 GetPosition() const { return mPos; }

        CS_EVENT_TYPE(EEventType::MouseMove);

    private:
        float2 mPos;
    };

    struct MouseScrollEvent : public WindowEvent
    {
        MouseScrollEvent(float32 offset, const DesktopWindowInfo& info, bool isPrimary)
            : mOffset(offset), WindowEvent(info, isPrimary)
        {
        }

        inline float32 GetOffset() const { return mOffset; }

        CS_EVENT_TYPE(EEventType::MouseScroll);

    private:
        float32 mOffset;
    };

    struct MouseButtonClickEvent : public WindowEvent
    {
        MouseButtonClickEvent(EMouseCode button, const DesktopWindowInfo& info, bool isPrimary)
            : mButton(button), WindowEvent(info, isPrimary)
        {
        }

        inline EMouseCode GetButton() const { return mButton; }

        CS_EVENT_TYPE(EEventType::MouseButtonClick);

    private:
        EMouseCode mButton;
    };
    struct MouseButtonReleaseEvent : public WindowEvent
    {
        MouseButtonReleaseEvent(EMouseCode button, const DesktopWindowInfo& info, bool isPrimary)
            : mButton(button), WindowEvent(info, isPrimary)
        {
        }

        inline EMouseCode GetButton() const { return mButton; }

        CS_EVENT_TYPE(EEventType::MouseButtonRelease);

    private:
        EMouseCode mButton;
    };

}
