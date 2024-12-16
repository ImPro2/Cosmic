#pragma once
#include "Base/Types.hpp"
// #include "Base/Tuples.hpp"
#include "App/Window/IWindow.hpp"
#include "App/Window/WindowInfo.hpp"
#include "EntryPoint/EntryPoint.hpp"
#include "Renderer/RendererAPI.hpp"

#include "Event/Events.hpp"

namespace Cosmic {

    struct ApplicationInfo
    {
        StartupArguments  StartupArgs        = { };
        const char*       Name               = "Cosmic Engine";
        DesktopWindowInfo WindowInfo         = DesktopWindowInfo();
        ERendererAPI      RendererBackend    = PlatformNativeRendererAPI();
        bool              EnableImGui        = true;
        String            ScriptAssemblyPath = "";
    };
    
    class Application
    {
    public:
        Application();
        ~Application();
        
        void Close();
    
    protected:
        void Init(ApplicationInfo&& info);
        void Init(const ApplicationInfo& info);
        void Shutdown();
    
    private:
        void Run();
    
    public:
        virtual void OnEvent(const IEvent &e);
    
    private:
        bool OnWindowResize(const WindowResizeEvent &e);
        bool OnWindowClose(const WindowCloseEvent &e);
    
    public:
        static Application*    Get()                      { return sInstance;     }
        const ApplicationInfo& GetInfo() const            { return mInfo;         }
        static void            Set(Application* instance) { sInstance = instance; }
        IDesktopWindow*        GetWindow()                { return mWindow.Ptr(); }
    
    private:
        static Application*   sInstance;
        bool                  mRunning   = true;
        bool                  mMinimized = false;
        ApplicationInfo       mInfo;
        Scope<IDesktopWindow> mWindow;
    };

}
