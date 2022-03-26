#include "cspch.hpp"
#include "EntryPoint/EntryPoint.hpp"
#include "App/Log/Log.hpp"

#include <Windows.h>
#include <iostream>

using namespace Cosmic;

CS_MODULE_LOG_INFO(Sandbox, SandboxApp);

import Cosmic.App;
import Cosmic.Base;
import Cosmic.App.IWindow;
import Cosmic.App.WindowInfo;
import Cosmic.App.Events;
import Cosmic.App.KeyAndMouseCodes;

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

    void OnEvent(const WindowEvent& e) override
    {
        Application::OnEvent(e);

        EventDispatcher dispatcher(e);
        CS_DISPATCH_EVENT(KeyPressEvent, OnKeyPress);
        CS_DISPATCH_EVENT(MouseButtonClickEvent, OnMouseButtonClick);
    }

    void OnKeyPress(const KeyPressEvent& e)
    {
        CS_LOG_INFO("OnKeyPress: KeyCode: {}, RepeatCount: {}", static_cast<char>(e.GetKeyCode()), e.GetRepeatCount());
    }

    void OnMouseButtonClick(const MouseButtonClickEvent& e)
    {
        switch (e.GetButton())
        {
            case MouseCode::ButtonLeft:   CS_LOG_INFO("MouseButton: Left");              break;
            case MouseCode::ButtonRight:  CS_LOG_INFO("MouseButton: Right");             break;
            case MouseCode::ButtonMiddle: CS_LOG_INFO("MouseButton: Middle");            break;
            default: CS_LOG_INFO("MouseButton: {}", static_cast<uint16>(e.GetButton())); break;
        }
    }
};

Cosmic::Application* Cosmic::CreateApplication()
{
    return new SandboxApp();
}