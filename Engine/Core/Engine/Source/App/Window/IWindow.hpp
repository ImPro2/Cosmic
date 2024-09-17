#pragma once
#include "Base/Base.hpp"
#include "App/Window/WindowInfo.hpp"
#include "App/Event/WindowEvents.hpp"
#include "Renderer/GraphicsContext.hpp"

namespace Cosmic
{

    using WindowHandle = void*;

    class IWindow
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

    class IDesktopWindow : public IWindow
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
        DesktopWindowInfo      mData;
        Scope<GraphicsContext> mGraphicsContext;
    };

    Scope<IDesktopWindow> CreateDesktopWindow(const DesktopWindowInfo& info);

}