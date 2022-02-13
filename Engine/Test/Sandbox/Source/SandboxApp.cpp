#include "cspch.hpp"
#include "EntryPoint/EntryPoint.hpp"
#include "App/Log/Log.hpp"

#include <Windows.h>
#include <iostream>

CS_MODULE_LOG_INFO(Sandbox, SandboxApp);

import Cosmic.App;
import Cosmic.Base;
import Cosmic.Window.IWindow;

class SandboxApp: public Cosmic::Application
{
public:
    SandboxApp()
    {
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