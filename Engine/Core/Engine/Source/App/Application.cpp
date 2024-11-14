#include "App/Application.hpp"
#include "cspch.hpp"
#include <functional>
// #include <optick.h>

CS_MODULE_LOG_INFO(Cosmic, App.Application);

#include "App/Event/AppEvents.hpp"
#include "App/FileSystem.hpp"
#include "App/Module.hpp"
#include "App/OS.hpp"
#include "Gui/Gui.hpp"
#include "Renderer/RenderCommand.hpp"
#include "Renderer/Renderer2D.hpp"
#include "Script/ScriptEngine.hpp"
#include "Time/Time.hpp"

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

        mInfo = std::move(info);

        OS::Init();
        RenderCommand::Init(mInfo.RendererBackend);

        ModuleSystem::Init();

        mWindow = CreateDesktopWindow(mInfo.WindowInfo);

        Renderer2D::Init();
        Gui::Init();
        FileSystem::Init("./");
        // ScriptEngine::Init(mInfo.ScriptAssemblyPath);

        OnEvent(ApplicationInitEvent(mInfo));
        // EventSystem::AddEvent(new ApplicationInitEvent(mInfo));

        Run();
    }
    
    void Application::Shutdown()
    {
        CS_PROFILE_FN();

        OnEvent(ApplicationCloseEvent());

        ScriptEngine::Shutdown();
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

        while (mRunning)
        {
            CS_PROFILE_RUN_LOOP("MainThread");

            Time::Update();

            if (!mMinimized)
            {
                ModuleSystem::OnUpdate();
                EventSystem::AddEvent(new ApplicationUpdateEvent());
                EventSystem::DispatchEvents();
            }

            if (mInfo.EnableImGui)
            {
                Gui::Begin();
                ModuleSystem::OnImGuiRender();
                Gui::End();
            }

            mWindow->Update();
        }

        Shutdown();
    }
    
    void Application::OnEvent(const Event &e)
    {
        CS_PROFILE_FN();

        EventDispatcher dispatcher(e);

        CS_DISPATCH_EVENT(WindowCloseEvent, OnWindowClose);
        CS_DISPATCH_EVENT(WindowResizeEvent, OnWindowResize);
        Gui::OnEvent((Event *)std::addressof(e));

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

} // namespace Cosmic
