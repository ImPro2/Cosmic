module;
#include "cspch.hpp"
#include <functional>
export module Cosmic.App.WindowEvents;

import Cosmic.Base;
import Cosmic.Base.Types;
import Cosmic.Base.Tuples;
import Cosmic.App.KeyAndMouseCodes;
import Cosmic.App.WindowInfo;

namespace Cosmic
{

    export enum class EWindowEventType
    {
        None = -1,
        WindowCreate, WindowClose, WindowResize,     WindowMove,        WindowTitle,
        KeyPress,     KeyRelease,  KeyType,
        MouseMove,    MouseScroll, MouseButtonClick, MouseButtonRelease
    };

    export struct WindowEvent
    {
    public:
        WindowEvent(const DesktopWindowInfo& info, bool isPrimary)
            : mInfo(info), mIsPrimary(isPrimary)
        {
        }

    public:
        inline bool                     IsPrimary() const { return mIsPrimary; }
        inline const DesktopWindowInfo& GetInfo()   const { return mInfo;      }

    public:
        virtual EWindowEventType GetType() = 0;

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
        virtual EWindowEventType GetType() override { return GetStaticType();                }
        static  EWindowEventType GetStaticType()    { return EWindowEventType::WindowCreate; }
    };
    export struct WindowCloseEvent : public WindowEvent
    {
    public:
        WindowCloseEvent(const DesktopWindowInfo& info, bool isPrimary)
            : WindowEvent(info, isPrimary)
        {
        }

    public:
        virtual EWindowEventType GetType() override { return GetStaticType();               }
        static  EWindowEventType GetStaticType()    { return EWindowEventType::WindowClose; }
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
        virtual EWindowEventType GetType() override { return GetStaticType();                }
        static  EWindowEventType GetStaticType()    { return EWindowEventType::WindowResize; }

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
        virtual EWindowEventType GetType() override { return GetStaticType();              }
        static  EWindowEventType GetStaticType()    { return EWindowEventType::WindowMove; }

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
        virtual EWindowEventType GetType() override { return GetStaticType();               }
        static  EWindowEventType GetStaticType()    { return EWindowEventType::WindowTitle; }

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
        virtual EWindowEventType GetType() override { return GetStaticType();            }
        static  EWindowEventType GetStaticType()    { return EWindowEventType::KeyPress; }

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
        virtual EWindowEventType GetType() override { return GetStaticType();              }
        static  EWindowEventType GetStaticType()    { return EWindowEventType::KeyRelease; }

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
        virtual EWindowEventType GetType() override { return GetStaticType();           }
        static  EWindowEventType GetStaticType()    { return EWindowEventType::KeyType; }

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
        virtual EWindowEventType GetType() override { return GetStaticType();             }
        static  EWindowEventType GetStaticType()    { return EWindowEventType::MouseMove; }

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
        virtual EWindowEventType GetType() override { return GetStaticType();               }
        static  EWindowEventType GetStaticType()    { return EWindowEventType::MouseScroll; }

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
        virtual EWindowEventType GetType() override { return GetStaticType();                    }
        static  EWindowEventType GetStaticType()    { return EWindowEventType::MouseButtonClick; }

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
        virtual EWindowEventType GetType() override { return GetStaticType();                      }
        static  EWindowEventType GetStaticType()    { return EWindowEventType::MouseButtonRelease; }

    private:
        EMouseCode mButton;
    };

}