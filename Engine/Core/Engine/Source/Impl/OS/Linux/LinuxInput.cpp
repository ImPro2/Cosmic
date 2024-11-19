#include "Base/Macros.hpp"
#include "cspch.hpp"

#ifdef CS_PLATFORM_LINUX

#include "LinuxUtils.hpp"
#include "App/Input.hpp"
#include "App/KeyAndMouseCodes.hpp"

#include <X11/XKBlib.h>

#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_X11
#include <GLFW/glfw3native.h>

CS_MODULE_LOG_INFO(Cosmic, Impl.OS.Linux.LinuxInput);

namespace Cosmic
{

    bool Input::IsKeyPressed(EKeyCode code)
    {
        int result = glfwGetKey(static_cast<GLFWwindow*>(Application::Get()->GetWindow()->GetHandle()), (int)code);
        return result == GLFW_PRESS || result == GLFW_REPEAT;
    }

    bool Input::IsMouseButtonPressed(EMouseCode button)
    {
        int result = glfwGetMouseButton(static_cast<GLFWwindow*>(Application::Get()->GetWindow()->GetHandle()), (int)button);
        return result == GLFW_PRESS || result == GLFW_REPEAT;
    }

    EKeyCode Input::GetKeyPressed()
    {
        //XkbStatePtr keyboardState;
        //XkbGetState(glfwGetX11Display(), XkbUseCoreKbd, keyboardState);

        CS_NOT_IMPLEMENTED();
        return (EKeyCode)-1;
    }

    EMouseCode Input::GetMouseButtonPressed()
    {
        CS_NOT_IMPLEMENTED();
        return (EMouseCode)-1;
    }

    float2 Input::GetMousePosition()
    {
        float64 x, y;
        glfwGetCursorPos(static_cast<GLFWwindow*>(Application::Get()->GetWindow()->GetHandle()), &x, &y);
        return { (float32)x, (float32)y };
    }

    void Input::SetCursorMode(ECursorMode mode)
    {
        CS_NOT_IMPLEMENTED();
    }

}

#endif
