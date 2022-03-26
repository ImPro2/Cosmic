module;
#include "cspch.hpp"
#include <functional>
export module Cosmic.App.IWindow;

import Cosmic.Base.Types;
import Cosmic.Base.Tuples;
import Cosmic.App.WindowInfo;
import Cosmic.App.WindowEvents;
import Cosmic.Renderer.GraphicsContext;

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

    export using WindowEventCallback = std::function<void(const WindowEvent&)>;

    // Exactly like DesktopWindowInfo, but also contains the event callback.
    export struct DesktopWindowData
    {
        WindowEventCallback Callback;
        union
        {
            DesktopWindowInfo Info; // In internal event functions to create events.
            struct
            {
                uint2  Size;
                uint2  Position;
                String Title;
                bool   IsVSync;
            };
        };

        DesktopWindowData(WindowEventCallback callback, DesktopWindowInfo info)
            : Callback(callback), Info(info)
        {
        }

        ~DesktopWindowData() {}

        template<typename T>
        void operator()(T event)
        {
            Callback(*static_cast<WindowEvent*>(&event));
        }
    };

    export class IDesktopWindow : public IWindow
    {
    public:
        IDesktopWindow(const DesktopWindowInfo& info, WindowEventCallback callback);
        virtual ~IDesktopWindow() { }

    public:
        virtual void Show()   = 0;
        virtual void Update() = 0;
        virtual void Close()  = 0;

    public:
        virtual WindowHandle GetHandle()   const            = 0;
        uint2                GetSize()     const { return mData.Size;     }
        uint2                GetPosition() const { return mData.Position; }
        const String&        GetTitle()    const { return mData.Title;    }
        bool                 IsVSync()     const { return mData.IsVSync;  }

        virtual void         SetSize(float2 size)           = 0;
        virtual void         SetPosition(float2 pos)        = 0;
        virtual void         SetTitle(const String& title)  = 0;
        virtual void         SetVSync(bool vsync)           = 0;

    protected:
        virtual void Init() = 0;

    protected:
        DesktopWindowData      mData;
        Scope<GraphicsContext> mGraphicsContext;
    };

    export Scope<IDesktopWindow> CreateDesktopWindow(const DesktopWindowInfo& info, WindowEventCallback callback);

}