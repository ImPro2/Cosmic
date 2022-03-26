module;
#include "cspch.hpp"
#include <functional>
module Cosmic.App.Application;
CS_MODULE_LOG_INFO(Cosmic, App.Application);

import Cosmic.App.Log;

namespace Cosmic
{

    Application::Application()
    {
        CS_ASSERT(!sInstance, "Already initialized the `Application` class.");
        sInstance = this;
    }

    Application::~Application()
    {

    }

    void Application::Init(ApplicationInfo&& info)
    {
        mInfo = std::move(info);

        mWindow = CreateDesktopWindow(mInfo.WindowInfo, [this](const WindowEvent& e)
        {
            return this->OnEvent(e);
        });

        ModuleSystem::Init();

        Run();
    }

    void Application::Shutdown()
    {
        mWindow->Close();
        ModuleSystem::Shutdown();
    }

    void Application::Close()
    {
        mRunning = false;
    }

    void Application::Run()
    {
        while (mRunning)
        {
            mWindow->Update();
        }

        Shutdown();
    }

    void Application::OnEvent(const WindowEvent& e)
    {
        EventDispatcher dispatcher(e);

        CS_DISPATCH_EVENT(WindowCloseEvent, OnWindowClose);
        CS_DISPATCH_EVENT(WindowResizeEvent, OnWindowResize);
    }

    void Application::OnWindowResize(const WindowResizeEvent& e)
    {
    }

    void Application::OnWindowClose(const WindowCloseEvent& e)
    {
        // Faster than an if statement.
        // Sets mRunning to false if it is the primary window (without branching).
        mRunning = !(e.IsPrimary());
    }

}