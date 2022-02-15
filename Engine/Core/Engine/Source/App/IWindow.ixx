module;
#include "cspch.hpp"
export module Cosmic.App.IWindow;

import Cosmic.Base.Types;
import Cosmic.Base.Tuples;

namespace Cosmic
{

    export using WindowHandle = void*;

    export class IWindow
    {
    public:
        IWindow()          { }
        virtual ~IWindow() { }

        template<typename T>
        Scope<T> As()
        {
            return Scope<T>(reinterpret_cast<T*>(this));
        }
    };

    export struct DesktopWindowInfo
    {
        uint2  Size;
        uint2  Position;
        String Title;
        bool   IsVSync;

        DesktopWindowInfo(
            uint2         size  = { 800, 600 },
            uint2         pos   = { 25,  25  },
            const String& title = "Cosmic Window",
            bool          vsync = false
        )
            : Size(size),
              Position(pos),
              Title(title),
              IsVSync(vsync)
        {
        }
    };

    export class IDesktopWindow : public IWindow
    {
    public:
        IDesktopWindow(const DesktopWindowInfo& info);
        virtual ~IDesktopWindow() { }

    public:
        virtual void Show()   = 0;
        virtual void Update() = 0;
        virtual void Close()  = 0;

    public:
        virtual WindowHandle GetHandle()   const            = 0;
        DesktopWindowInfo    GetInfo()     const { return mInfo;          }
        uint2                GetSize()     const { return mInfo.Size;     }
        uint2                GetPosition() const { return mInfo.Position; }
        const String&        GetTitle()    const { return mInfo.Title;    }
        bool                 IsVSync()     const { return mInfo.IsVSync;  }

        virtual void         SetSize(float2 size)           = 0;
        virtual void         SetPosition(float2 pos)        = 0;
        virtual void         SetTitle(const String& title)  = 0;
        virtual void         SetVSync(bool vsync)           = 0;

    protected:
        virtual void Init() = 0;

    protected:
        DesktopWindowInfo mInfo;
    };

    export Scope<IDesktopWindow> CreateDesktopWindow(const DesktopWindowInfo& info);

}