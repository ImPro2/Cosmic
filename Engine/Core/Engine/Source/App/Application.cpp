module;
#include "cspch.hpp"
#include <functional>
#include <optick.h>
module Cosmic.App.Application;
CS_MODULE_LOG_INFO(Cosmic, App.Application);

import Cosmic.App.Log;
import Cosmic.App.AppEvents;
import Cosmic.App.Module;
import Cosmic.App.AppEvents;

namespace Cosmic
{

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

        mWindow = CreateDesktopWindow(mInfo.WindowInfo, [this](const WindowEvent& e) { return this->OnEvent(e); });
        ModuleSystem::Init();

        OnEvent(ApplicationInitEvent(mInfo));

        Run();
    }

    void Application::Shutdown()
    {
        CS_PROFILE_FN();

        OnEvent(ApplicationCloseEvent());

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

            OnEvent(ApplicationUpdateEvent());

            ModuleSystem::OnUpdate();
            mWindow->Update();
        }

        Shutdown();
    }

    void Application::OnEvent(const Event& e)
    {
        CS_PROFILE_FN();

        EventDispatcher dispatcher(e);

        CS_DISPATCH_EVENT(WindowCloseEvent, OnWindowClose);
        CS_DISPATCH_EVENT(WindowResizeEvent, OnWindowResize);

        ModuleSystem::OnEvent(e);
    }

    void Application::OnWindowResize(const WindowResizeEvent& e)
    {
        CS_PROFILE_FN();
    }

    void Application::OnWindowClose(const WindowCloseEvent& e)
    {
        CS_PROFILE_FN();

        if (e.IsPrimary())
            Close();
    }

}