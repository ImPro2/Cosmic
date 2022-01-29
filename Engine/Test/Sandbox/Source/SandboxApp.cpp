#include "cspch.hpp"
#include "EntryPoint/EntryPoint.hpp"
#include "App/Log/Log.hpp"

#include <Windows.h>
#include <iostream>

CS_MODULE_LOG_INFO(Sandbox, SandboxApp);

import Cosmic.App;

class SandboxApp: public Cosmic::Application
{
public:
    SandboxApp()
    {
        Init({
            .Name = "TestSandbox"
        });
    }
};

Cosmic::Application* Cosmic::CreateApplication()
{
    return new SandboxApp();
}