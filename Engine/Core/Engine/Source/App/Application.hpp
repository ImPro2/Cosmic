#pragma once
#include "Base/Types.hpp"
// #include "Base/Tuples.hpp"
#include "App/Window/IWindow.hpp"
#include "App/Window/WindowInfo.hpp"
#include "EntryPoint/EntryPoint.hpp"
#include "Renderer/RendererAPI.hpp"

#include "Event/Events.hpp"

#include "App/Log/Log.hpp"
#include "App/FileSystem.hpp"
#include "App/Module.hpp"
#include "Event/EventSystem.hpp"
#include "Gui/Gui.hpp"
#include "Memory/Allocations.hpp"
#include "Project/ProjectManager.hpp"
#include "Renderer/RenderCommand.hpp"
#include "Renderer/Renderer2D.hpp"
#include "Script/NativeScriptEngine.hpp"
#include "Time/Time.hpp"

namespace Cosmic
{

    enum class EEngineSystem : uint32
    {
        None               = 0,
        OS                 = CS_BIT(0),
        Allocations        = CS_BIT(1),
        Logging            = CS_BIT(2),
        Events             = CS_BIT(3),
        Renderer           = CS_BIT(4),
        Modules            = CS_BIT(5),
        AppWindow          = CS_BIT(6),
        GUI                = CS_BIT(7),
        FileSystem         = CS_BIT(8),
        NativeScriptEngine = CS_BIT(9),
        ProjectManager     = CS_BIT(10),
        Time               = CS_BIT(11),

        All = OS | Allocations | Logging | Events | Renderer | Modules | AppWindow | GUI | FileSystem | NativeScriptEngine | ProjectManager | Time
    };

    struct ApplicationInfo
    {
        StartupArgumentList     StartupArgs        = { };
        const char*             Name               = "Cosmic Engine";
        BitFlags<EEngineSystem> EnabledSystems     = EEngineSystem::All;
        DesktopWindowInfo       WindowInfo         = DesktopWindowInfo();
        ERendererAPI            RendererBackend    = PlatformNativeRendererAPI();
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
        IDesktopWindow*        GetWindow()                { return mWindow.Ptr(); }

    public:
        static void Set(Application* instance);
    
    private:
        static Application*   sInstance;
        bool                  mRunning   = true;
        bool                  mMinimized = false;
        ApplicationInfo       mInfo;
        Scope<IDesktopWindow> mWindow;

        PersistentRef<Log>                mLogInstance;
        PersistentRef<FileSystem>         mFileSystemInstance;
        PersistentRef<ModuleSystem>       mModuleSystemInstance;
        PersistentRef<EventSystem>        mEventSystemInstance;
        PersistentRef<Gui>                mGuiInstance;
        PersistentRef<Allocations>        mAllocationsInstance;
        PersistentRef<ProjectManager>     mProjectManagerInstance;
        PersistentRef<RendererAPI>        mRendererAPIInstance;
        PersistentRef<Renderer2D>         mRenderer2DInstance;
        PersistentRef<NativeScriptEngine> mNativeScriptEngineInstance;
        PersistentRef<Time>               mTimeInstance;
    };

}
