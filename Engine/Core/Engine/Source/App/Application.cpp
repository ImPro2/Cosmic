#include "App/Application.hpp"
#include "cspch.hpp"
#include <functional>
// #include <optick.h>

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

CS_MODULE_LOG_INFO(Cosmic, App.Application);

namespace Cosmic
{

    namespace Utils
    {

        void CheckEngineSystemsValidity(const BitFlags<EEngineSystem>& enabled)
        {
            if (enabled.IsSet(EEngineSystem::All))
                return;

            if (!enabled.IsSet(EEngineSystem::Allocations))
                CS_ASSERT(false, "Engine Allocations need to be enabled");

            if (enabled.IsSet(EEngineSystem::Renderer) && !enabled.IsSet(EEngineSystem::AppWindow))
                CS_ASSERT(false, "Renderer requires AppWindow");

            if (enabled.IsSet(EEngineSystem::Modules) && !enabled.IsSet(EEngineSystem::Events))
                CS_ASSERT(false, "Modules requires Events");

            if (enabled.IsSet(EEngineSystem::AppWindow) && (!enabled.IsSet(EEngineSystem::Renderer) || !enabled.IsSet(EEngineSystem::Modules) || !enabled.IsSet(EEngineSystem::Events)))
                CS_ASSERT(false, "AppWindow requires Renderer, Modules and Events");

            if (enabled.IsSet(EEngineSystem::GUI) && (!enabled.IsSet(EEngineSystem::AppWindow) || !enabled.IsSet(EEngineSystem::Renderer) || !enabled.IsSet(EEngineSystem::Modules) || !enabled.IsSet(EEngineSystem::Events)))
                CS_ASSERT(false, "GUI requires AppWindow, Renderer, Modules and Events");

            if (enabled.IsSet(EEngineSystem::FileSystem) && !enabled.IsSet(EEngineSystem::Events))
                CS_ASSERT(false, "FileSystem requires Events");
        }

    }

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
    
    void Application::Init(const ApplicationInfo& info)
    {
        CS_PROFILE_FN();

        if (!mRunning)
            return;

        mInfo = info;

        Utils::CheckEngineSystemsValidity(info.EnabledSystems);

        if (mInfo.EnabledSystems.IsSet(EEngineSystem::Allocations))
			mAllocationsInstance = Allocations::Init();

        if (mInfo.EnabledSystems.IsSet(EEngineSystem::OS))
			OS::Init();

        if (mInfo.EnabledSystems.IsSet(EEngineSystem::Logging))
			mLogInstance = Log::Init();

        if (mInfo.EnabledSystems.IsSet(EEngineSystem::Events))
			mEventSystemInstance = EventSystem::Init();

        if (mInfo.EnabledSystems.IsSet(EEngineSystem::Renderer))
			mRendererAPIInstance = RenderCommand::Init(mInfo.RendererBackend);

        if (mInfo.EnabledSystems.IsSet(EEngineSystem::Modules))
			mModuleSystemInstance = ModuleSystem::Init();

        if (mInfo.EnabledSystems.IsSet(EEngineSystem::AppWindow))
			mWindow = CreateDesktopWindow(mInfo.WindowInfo);

        if (mInfo.EnabledSystems.IsSet(EEngineSystem::Renderer))
			mRenderer2DInstance = Renderer2D::Init();

        if (mInfo.EnabledSystems.IsSet(EEngineSystem::GUI))
			mGuiInstance = Gui::Init();

        if (mInfo.EnabledSystems.IsSet(EEngineSystem::FileSystem))
			mFileSystemInstance = FileSystem::Init(String("./"));

        if (mInfo.EnabledSystems.IsSet(EEngineSystem::NativeScriptEngine))
			mNativeScriptEngineInstance = NativeScriptEngine::Init();

        if (mInfo.EnabledSystems.IsSet(EEngineSystem::ProjectManager))
			mProjectManagerInstance = ProjectManager::Init();

        if (mInfo.EnabledSystems.IsSet(EEngineSystem::Time))
			mTimeInstance = Time::Init();

        if (mInfo.EnabledSystems.IsSet(EEngineSystem::Events))
			OnEvent(ApplicationInitEvent(mInfo));

        Run();
    }   

    void Application::Shutdown()
    {
        CS_PROFILE_FN();

        if (mInfo.EnabledSystems.IsSet(EEngineSystem::Events))
			OnEvent(ApplicationCloseEvent());

        if (mInfo.EnabledSystems.IsSet(EEngineSystem::Events))
            Time::Shutdown();

        if (mInfo.EnabledSystems.IsSet(EEngineSystem::ProjectManager))
            ProjectManager::Shutdown();

        if (mInfo.EnabledSystems.IsSet(EEngineSystem::NativeScriptEngine))
			NativeScriptEngine::Shutdown();

        if (mInfo.EnabledSystems.IsSet(EEngineSystem::FileSystem))
            FileSystem::Shutdown();

        if (mInfo.EnabledSystems.IsSet(EEngineSystem::GUI))
            Gui::Shutdown();

        if (mInfo.EnabledSystems.IsSet(EEngineSystem::AppWindow))
            mWindow->Close();

        if (mInfo.EnabledSystems.IsSet(EEngineSystem::AppWindow))
            ModuleSystem::Shutdown();

        if (mInfo.EnabledSystems.IsSet(EEngineSystem::AppWindow))
        {
            RenderCommand::Shutdown();
            Renderer2D::Shutdown();
        }

        if (mInfo.EnabledSystems.IsSet(EEngineSystem::Events))
			EventSystem::Shutdown();

        if (mInfo.EnabledSystems.IsSet(EEngineSystem::Logging))
            Log::Shutdown();

        if (mInfo.EnabledSystems.IsSet(EEngineSystem::OS))
            OS::Shutdown();

        if (mInfo.EnabledSystems.IsSet(EEngineSystem::Allocations))
            Allocations::Shutdown();
    }
    
    void Application::Close()
    {
        CS_PROFILE_FN();

        mRunning = false;
    }
    
    void Application::Run()
    {
        CS_PROFILE_FN();

        while (mRunning)
        {
            CS_PROFILE_RUN_LOOP("MainThread");

            if (mInfo.EnabledSystems.IsSet(EEngineSystem::Time))
				Time::Update();

            if (!mMinimized)
            {
				if (mInfo.EnabledSystems.IsSet(EEngineSystem::Modules))
					ModuleSystem::OnUpdate();

				if (mInfo.EnabledSystems.IsSet(EEngineSystem::Events))
					EventSystem::DeferEvent<ApplicationUpdateEvent>();
            }

            if (mInfo.EnabledSystems.IsSet(EEngineSystem::GUI))
            {
                Gui::Begin();
                ModuleSystem::OnImGuiRender();
                Gui::End();
            }

            if (mInfo.EnabledSystems.IsSet(EEngineSystem::AppWindow))
				mWindow->Update();

            if (mInfo.EnabledSystems.IsSet(EEngineSystem::Events))
				EventSystem::DispatchEvents();

            if (mInfo.EnabledSystems.IsSet(EEngineSystem::Allocations))
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

        if (mInfo.EnabledSystems.IsSet(EEngineSystem::GUI))
			Gui::OnEvent((IEvent*)std::addressof(e));

        if (mInfo.EnabledSystems.IsSet(EEngineSystem::Modules))
			ModuleSystem::OnEvent(e);
    }
    
    bool Application::OnWindowResize(const WindowResizeEvent &e)
    {
        CS_PROFILE_FN();

        if (e.GetWidth() == 0 || e.GetHeight() == 0)
            mMinimized = true;
        else
            mMinimized = false;

        if (mInfo.EnabledSystems.IsSet(EEngineSystem::Renderer))
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
