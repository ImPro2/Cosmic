#include "App/Application.hpp"
#include "cspch.hpp"
#include <functional>
// #include <optick.h>

CS_MODULE_LOG_INFO(Cosmic, App.Application);

#include "Event/Type/AppEvents.hpp"
#include "App/FileSystem.hpp"
#include "App/Module.hpp"
#include "App/OS.hpp"
#include "Gui/Gui.hpp"
#include "Renderer/RenderCommand.hpp"
#include "Renderer/Renderer2D.hpp"
#include "Script/NativeScriptEngine.hpp"
#include "Time/Time.hpp"

#include "Memory/Memory.hpp"

namespace Cosmic
{

    Application* Application::sInstance = nullptr;
    
    Application::Application()
    {
        CS_PROFILE_FN();

        CS_ASSERT(!sInstance, "Already initialized the `Application` class.");
        sInstance = this;
    }
    
    Application::~Application()
    {
    }
    
    void Application::Init(ApplicationInfo&& info)
    {
        CS_PROFILE_FN();

        Init(std::move(info));
    }

    void Application::Init(const ApplicationInfo& info)
    {
        CS_PROFILE_FN();

        mInfo = info;

        OS::Init();

        mAllocationsInstance  = Allocations::Init();
        mLogInstance          = Log::Init();
        mEventSystemInstance  = EventSystem::Init();
        mRendererAPIInstance  = RenderCommand::Init(mInfo.RendererBackend);
        mModuleSystemInstance = ModuleSystem::Init();

        mWindow = CreateDesktopWindow(mInfo.WindowInfo);

        mRenderer2DInstance = Renderer2D::Init();
        mGuiInstance        = Gui::Init();
        mFileSystemInstance = FileSystem::Init(String("./"));

        mNativeScriptEngineInstance = NativeScriptEngine::Init(mInfo.ScriptAssemblyPath);

        mProjectManagerInstance = ProjectManager::Init();
        mTimeInstance = Time::Init();

        OnEvent(ApplicationInitEvent(mInfo));

        Run();
    }   

    void Application::Shutdown()
    {
        CS_PROFILE_FN();

        OnEvent(ApplicationCloseEvent());

        Allocations::Shutdown();
        NativeScriptEngine::Shutdown();
        Gui::Shutdown();
        Renderer2D::Shutdown();
        RenderCommand::Shutdown();
        mWindow->Close();
        ModuleSystem::Shutdown();
    }
    
    void Application::Close()
    {
        CS_PROFILE_FN();

        mRunning = false;
    }
    
    void Application::Run()
    {
        CS_PROFILE_FN();

		TimeUnit lastTime = Time::GetTime();

        while (mRunning)
        {
            CS_PROFILE_RUN_LOOP("MainThread");

            Time::Update();

            if (!mMinimized)
            {
                ModuleSystem::OnUpdate();
                EventSystem::DeferEvent<ApplicationUpdateEvent>();
            }

            if (mInfo.EnableImGui)
            {
                Gui::Begin();
                ModuleSystem::OnImGuiRender();
                Gui::End();
            }

            mWindow->Update();

			EventSystem::DispatchEvents();
            Allocations::EndFrame();
        }

        Shutdown();
    }
    
    void Application::OnEvent(const IEvent &e)
    {
        CS_PROFILE_FN();

        EventDispatcher dispatcher(e);

        CS_DISPATCH_EVENT(WindowCloseEvent, OnWindowClose);
        CS_DISPATCH_EVENT(WindowResizeEvent, OnWindowResize);
        Gui::OnEvent((IEvent*)std::addressof(e));

        ModuleSystem::OnEvent(e);
    }
    
    bool Application::OnWindowResize(const WindowResizeEvent &e)
    {
        CS_PROFILE_FN();

        if (e.GetWidth() == 0 || e.GetHeight() == 0)
            mMinimized = true;
        else
            mMinimized = false;

        RenderCommand::SetViewport(0, 0, e.GetWidth(), e.GetHeight());

        return false;
    }
    
    bool Application::OnWindowClose(const WindowCloseEvent &e)
    {
        CS_PROFILE_FN();

        if (e.IsPrimary())
            Close();

        return true;
    }

    void Application::Set(Application* instance)
    {
        sInstance = instance;

        Allocations::SetInstance(sInstance->mAllocationsInstance);

        Log::sInstance                = sInstance->mLogInstance;
        FileSystem::sInstance         = sInstance->mFileSystemInstance;
        ModuleSystem::sInstance       = sInstance->mModuleSystemInstance;
        EventSystem::sInstance        = sInstance->mEventSystemInstance;
        Gui::sInstance                = sInstance->mGuiInstance;
        ProjectManager::sInstance     = sInstance->mProjectManagerInstance;
        RenderCommand::sAPI           = sInstance->mRendererAPIInstance;
        Renderer2D::sInstance         = sInstance->mRenderer2DInstance;
        NativeScriptEngine::sInstance = sInstance->mNativeScriptEngineInstance;
        Time::sInstance               = sInstance->mTimeInstance;
    }

}
