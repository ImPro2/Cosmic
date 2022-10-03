module;
#include "cspch.hpp"
export module Cosmic.App.Application;

import Cosmic.Base.Types;
import Cosmic.Base.Tuples;
import Cosmic.App.Events;
import Cosmic.App.WindowInfo;
import Cosmic.App.IWindow;
import Cosmic.Renderer.RendererAPI;
import Cosmic.Renderer.RenderCommand;
import Cosmic.App.Events;
import Cosmic.App.WindowEvents;
import Cosmic.Time;
import Cosmic.App.OS;

namespace Cosmic
{

    export struct ApplicationInfo
    {
        const char*       Name            = "Cosmic Engine";
        DesktopWindowInfo WindowInfo      = DesktopWindowInfo();
        ERendererAPI      RendererBackend = PlatformNativeRendererAPI();
    };

    export class Application
    {
    public:
        Application();
        ~Application();

        void Close();

    protected:
        void Init(ApplicationInfo&& info);
        void Shutdown();

    private:
        void Run();

    protected:
        // Make this method protected so that the class that derives from this class
        // can call this method when it overrides it.
        virtual void OnEvent(const Event& e);

    private:
        void OnWindowResize(const WindowResizeEvent& e);
        void OnWindowClose(const WindowCloseEvent& e);

    public:
        static Application&   Get() { return *sInstance;    }
        IDesktopWindow* GetWindow() { return mWindow.get(); }

    private:
        inline static Application* sInstance = nullptr;
        bool                       mRunning  = true;
        ApplicationInfo            mInfo;
        Scope<IDesktopWindow>      mWindow;
    };

}