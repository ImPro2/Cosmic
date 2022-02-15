#include "cspch.hpp"
#include "EntryPoint/EntryPoint.hpp"
#include "App/Log/Log.hpp"

#include <Windows.h>
#include <iostream>

CS_MODULE_LOG_INFO(Sandbox, SandboxApp);

import Cosmic.App;
import Cosmic.Base;
import Cosmic.App.IWindow;
import Cosmic.App.EventSystem;

class SandboxApp: public Cosmic::Application
{
public:
    SandboxApp()
    {
        Cosmic::EventSystem::Submit<Cosmic::Struct0>({ 69 });
        Cosmic::EventSystem::Submit<Cosmic::Struct1>({ "Hello, World!"});

        Cosmic::EventSystem::On<Cosmic::Struct0>([](Cosmic::Struct0& e) {
            CS_LOG_INFO("{}", e.i);
        });

        Cosmic::EventSystem::On<Cosmic::Struct1>([](Cosmic::Struct1& e) {
            CS_LOG_INFO("{}", e.c);
        });

        Cosmic::EventSystem::Update();

        Init({
            .Name       = "TestSandbox",
            .WindowInfo = Cosmic::DesktopWindowInfo()
        });
    }
};

Cosmic::Application* Cosmic::CreateApplication()
{
    return new SandboxApp();
}