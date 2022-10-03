module;
#include "cspch.hpp"
#include <functional>
export module Cosmic.App.WindowEvents;

import Cosmic.App.Events;
import Cosmic.Base;
import Cosmic.Base.Types;
import Cosmic.Base.Tuples;
import Cosmic.App.KeyAndMouseCodes;
import Cosmic.App.WindowInfo;

namespace Cosmic
{

    export struct WindowEvent : public Event
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

    export struct WindowCreateEvent : public WindowEvent
    {
    public:
        WindowCreateEvent(const DesktopWindowInfo& info, bool isPrimary)
            : WindowEvent(info, isPrimary)
        {
        }

    public:
        virtual EEventType GetType() const  override { return GetStaticType();                }
        static  EEventType GetStaticType()    { return EEventType::WindowCreate; }
    };
    export struct WindowCloseEvent : public WindowEvent
    {
    public:
        WindowCloseEvent(const DesktopWindowInfo& info, bool isPrimary)
            : WindowEvent(info, isPrimary)
        {
        }

    public:
        virtual EEventType GetType() const  const  override { return GetStaticType();               }
        static  EEventType GetStaticType()    { return EEventType::WindowClose; }
    };
    export struct WindowResizeEvent : public WindowEvent
    {
    public:
        WindowResizeEvent(uint2 size, const DesktopWindowInfo& info, bool isPrimary)
            : mSize(size), WindowEvent(info, isPrimary)
        {
        }

    public:
        inline uint2  GetSize()   const { return mSize;        }
        inline uint32 GetWidth()  const { return mSize.width;  }
        inline uint32 GetHeight() const { return mSize.height; }


    public:
        virtual EEventType GetType() const override { return GetStaticType();                }
        static  EEventType GetStaticType()    { return EEventType::WindowResize; }

    private:
        uint2 mSize;
    };
    export struct WindowMoveEvent : public WindowEvent
    {
    public:
        WindowMoveEvent(int2 pos, const DesktopWindowInfo& info, bool isPrimary)
            : mPos(pos), WindowEvent(info, isPrimary)
        {
        }

    public:
        inline int2  GetPosition()  const { return mPos;   }
        inline int32 GetXPosition() const { return mPos.x; }
        inline int32 GetYPosition() const { return mPos.y; }

    public:
        virtual EEventType GetType() const override { return GetStaticType();              }
        static  EEventType GetStaticType()    { return EEventType::WindowMove; }

    private:
       int2 mPos;
    };
    export struct WindowTitleEvent : public WindowEvent
    {
    public:
        WindowTitleEvent(const String& title, const DesktopWindowInfo& info, bool isPrimary)
            : mTitle(title), WindowEvent(info, isPrimary)
        {
        }

    public:
        inline const String& GetTitle()  const { return mTitle; }

    public:
        virtual EEventType GetType() const  override { return GetStaticType();               }
        static  EEventType GetStaticType()    { return EEventType::WindowTitle; }

    public:
        String mTitle;
    };
    export struct KeyPressEvent : public WindowEvent
    {
    public:
        KeyPressEvent(EKeyCode key, uint8 repeatCount, const DesktopWindowInfo& info, bool isPrimary)
            : mKey(key), mRepeatCount(repeatCount), WindowEvent(info, isPrimary)
        {
        }

    public:
        inline EKeyCode GetKeyCode()     const { return mKey;         }
        inline uint8    GetRepeatCount() const { return mRepeatCount; }

    public:
        virtual EEventType GetType() const  override { return GetStaticType();            }
        static  EEventType GetStaticType()    { return EEventType::KeyPress; }

    private:
        EKeyCode mKey;
        uint8    mRepeatCount;
    };
    export struct KeyReleaseEvent : public WindowEvent
    {
    public:
        KeyReleaseEvent(EKeyCode key, const DesktopWindowInfo& info, bool isPrimary)
            : mKey(key), WindowEvent(info, isPrimary)
        {
        }

    public:
        inline EKeyCode GetKeyCode() const { return mKey; }

    public:
        virtual EEventType GetType() const  override { return GetStaticType();              }
        static  EEventType GetStaticType()    { return EEventType::KeyRelease; }

    private:
        EKeyCode mKey;
    };
    export struct KeyTypeEvent : public WindowEvent
    {
    public:
        KeyTypeEvent(char character, const DesktopWindowInfo& info, bool isPrimary)
            : mChar(character), WindowEvent(info, isPrimary)
        {
        }

    public:
        inline char GetChar() const { return mChar; }

    public:
        virtual EEventType GetType() const  override { return GetStaticType();           }
        static  EEventType GetStaticType()    { return EEventType::KeyType; }

    private:
        char mChar;
    };
    export struct MouseMoveEvent : public WindowEvent
    {
    public:
        MouseMoveEvent(float2 pos, const DesktopWindowInfo& info, bool isPrimary)
            : mPos(pos), WindowEvent(info, isPrimary)
        {
        }

    public:
        inline float2 GetPosition() const { return mPos; }

    public:
        virtual EEventType GetType() const  override { return GetStaticType();             }
        static  EEventType GetStaticType()    { return EEventType::MouseMove; }

    private:
        float2 mPos;
    };
    export struct MouseScrollEvent : public WindowEvent
    {
    public:
        MouseScrollEvent(float32 offset, const DesktopWindowInfo& info, bool isPrimary)
            : mOffset(offset), WindowEvent(info, isPrimary)
        {
        }

    public:
        inline float32 GetOffset() const { return mOffset; }

    public:
        virtual EEventType GetType() const  override { return GetStaticType();               }
        static  EEventType GetStaticType()    { return EEventType::MouseScroll; }

    private:
        float32 mOffset;
    };
    export struct MouseButtonClickEvent : public WindowEvent
    {
    public:
        MouseButtonClickEvent(EMouseCode button, const DesktopWindowInfo& info, bool isPrimary)
            : mButton(button), WindowEvent(info, isPrimary)
        {
        }

    public:
        inline EMouseCode GetButton() const { return mButton; }

    public:
        virtual EEventType GetType() const  override { return GetStaticType();                    }
        static  EEventType GetStaticType()    { return EEventType::MouseButtonClick; }

    private:
        EMouseCode mButton;
    };
    export struct MouseButtonReleaseEvent : public WindowEvent
    {
    public:
        MouseButtonReleaseEvent(EMouseCode button, const DesktopWindowInfo& info, bool isPrimary)
            : mButton(button), WindowEvent(info, isPrimary)
        {
        }

    public:
        inline EMouseCode GetButton() const { return mButton; }

    public:
        virtual EEventType GetType() const  override { return GetStaticType();                      }
        static  EEventType GetStaticType()    { return EEventType::MouseButtonRelease; }

    private:
        EMouseCode mButton;
    };

}