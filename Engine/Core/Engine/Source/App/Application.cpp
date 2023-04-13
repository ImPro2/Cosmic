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
import Cosmic.Gui;
import Cosmic.Renderer.RenderCommand;
import Cosmic.Time;
import Cosmic.Renderer.Renderer2D;
import Cosmic.Script.ScriptEngine;
import Cosmic.App.FileSystem;
import Cosmic.App.OS;

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

        mWindow = CreateDesktopWindow(mInfo.WindowInfo, [this](const WindowEvent& e) { return this->OnEvent(e); });

        Renderer2D::Init();
        Gui::Init();
        FileSystem::Init("C:/Dev/Cosmic");
        ScriptEngine::Init(mInfo.ScriptAssemblyPath);

        OnEvent(ApplicationInitEvent(mInfo));

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
                OnEvent(ApplicationUpdateEvent());
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

    void Application::OnEvent(const Event& e)
    {
        CS_PROFILE_FN();

        EventDispatcher dispatcher(e);

        CS_DISPATCH_EVENT(WindowCloseEvent, OnWindowClose);
        CS_DISPATCH_EVENT(WindowResizeEvent, OnWindowResize);
        Gui::OnEvent((Event*)std::addressof(e));

        ModuleSystem::OnEvent(e);
    }

    bool Application::OnWindowResize(const WindowResizeEvent& e)
    {
        CS_PROFILE_FN();

        if (e.GetWidth() == 0 || e.GetHeight() == 0)
            mMinimized = true;
        else
            mMinimized = false;

        RenderCommand::SetViewport(0, 0, e.GetWidth(), e.GetHeight());

        return false;
    }

    bool Application::OnWindowClose(const WindowCloseEvent& e)
    {
        CS_PROFILE_FN();

        if (e.IsPrimary())
            Close();

        return true;
    }

}