#include "cspch.hpp"

#ifdef CS_PLATFORM_WINDOWS

#include "IWindowsWindow.hpp"
#include "WindowsUtils.hpp"
//#include "glad/glad.h"
#include "GLFW/glfw3.h"
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
#include <functional>

CS_MODULE_LOG_INFO(Cosmic, Impl.OS.Windows.IWindowsWindow);

#include "App/Log/Log.hpp"
#include "Base/Types.hpp"
#include "Base/Tuples.hpp"
#include "App/Window/IWindow.hpp"
#include "App/Window/WindowInfo.hpp"
#include "Event/EventSystem.hpp"
#include "Event/Type/WindowEvents.hpp"
#include "App/KeyAndMouseCodes.hpp"

namespace Cosmic
{

    static uint32 sGLFWWindowCount = 0;

    IWindowsDesktopWindow::IWindowsDesktopWindow(const DesktopWindowInfo& info)
        : IDesktopWindow(info)
    {
        CS_PROFILE_FN();

        CS_LOG_INFO("Creating window {0}: [{1}:{2}]", mData.Title.c_str(), mData.Size.width, mData.Size.height);

        Init();
        Show();
    }

    IWindowsDesktopWindow::~IWindowsDesktopWindow()
    {
        CS_PROFILE_FN();

        Close();
    }

    void IWindowsDesktopWindow::Init()
    {
        CS_PROFILE_FN();

        if (sGLFWWindowCount == 0)
            CS_ASSERT(glfwInit() == GLFW_TRUE, "Failed to initialize GLFW.");

        sGLFWWindowCount++;

        mHandle = glfwCreateWindow((int)mData.Size.width, (int)mData.Size.height, mData.Title.c_str(), nullptr, nullptr);

        mGraphicsContext = CreateGraphicsContext((void*)mHandle);
        mGraphicsContext->Init();

        SetupCallbacks();

        EventSystem::DeferEvent<WindowCreateEvent>(mData, true);
    }

    void IWindowsDesktopWindow::SetupCallbacks()
    {
        CS_PROFILE_FN();

        glfwSetWindowUserPointer(mHandle, &mData);

        glfwSetWindowCloseCallback(mHandle, [](GLFWwindow* window)
		{
            DesktopWindowInfo& data = *(DesktopWindowInfo*)glfwGetWindowUserPointer(window);

            EventSystem::DeferEvent<WindowCloseEvent>(data, true);
        });

        glfwSetWindowSizeCallback(mHandle, [](GLFWwindow* window, int width, int height)
		{
            DesktopWindowInfo& data = *(DesktopWindowInfo*)glfwGetWindowUserPointer(window);
            data.Size.width  = (uint32)width;
            data.Size.height = (uint32)height;

            EventSystem::DeferEvent<WindowResizeEvent>(data.Size, data, true);
        });

        glfwSetKeyCallback(mHandle, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
            DesktopWindowInfo& data = *(DesktopWindowInfo*)glfwGetWindowUserPointer(window);
            static int repeatCount  = 0;

            switch (action)
            {
                case GLFW_REPEAT:
                {
                    repeatCount++;
                    EventSystem::DeferEvent<KeyPressEvent>((EKeyCode)key, repeatCount, data, true);
                    break;
                }
                case GLFW_PRESS:
                {
                    EventSystem::DeferEvent<KeyPressEvent>((EKeyCode)key, repeatCount, data, true);
                    break;
                }
                case GLFW_RELEASE:
                {
                    EventSystem::DeferEvent<KeyReleaseEvent>((EKeyCode)key, data, true);
                    repeatCount = 0;
                    break;
                }
            }
        });

        glfwSetCharCallback(mHandle, [](GLFWwindow* window, unsigned int keycode)
        {
            DesktopWindowInfo& data = *(DesktopWindowInfo*)glfwGetWindowUserPointer(window);
            EventSystem::DeferEvent<KeyTypeEvent>((char)keycode, data, true);
        });

        glfwSetCursorPosCallback(mHandle, [](GLFWwindow* window, double xpos, double ypos)
		{
			DesktopWindowInfo& data = *(DesktopWindowInfo*)glfwGetWindowUserPointer(window);
			EventSystem::DeferEvent<MouseMoveEvent>(float2 { (float32)xpos, (float32)ypos }, data, true);
        });

        glfwSetScrollCallback(mHandle, [](GLFWwindow* window, double xoffset, double yoffset)
		{
            DesktopWindowInfo& data = *(DesktopWindowInfo*)glfwGetWindowUserPointer(window);
            EventSystem::DeferEvent<MouseScrollEvent>((float32)yoffset, data, true);
        });

        glfwSetMouseButtonCallback(mHandle, [](GLFWwindow* window, int button, int action, int mods)
		{
            DesktopWindowInfo& data = *(DesktopWindowInfo*)glfwGetWindowUserPointer(window);

            switch (action)
            {
                case GLFW_PRESS:
                    EventSystem::DeferEvent<MouseButtonClickEvent>((EMouseCode)button, data, true);
                case GLFW_RELEASE:
                    EventSystem::DeferEvent<MouseButtonReleaseEvent>((EMouseCode)button, data, true);
            }
        });
    }

    void IWindowsDesktopWindow::Show()
    {
        CS_PROFILE_FN();

        glfwShowWindow(mHandle);
    }

    void IWindowsDesktopWindow::Update()
    {
        CS_PROFILE_FN();

        glfwPollEvents();
        mGraphicsContext->SwapBuffers();
    }

    void IWindowsDesktopWindow::Close()
    {
        CS_PROFILE_FN();

        glfwDestroyWindow(mHandle);

        sGLFWWindowCount--;

        if (sGLFWWindowCount == 0)
        {
            glfwTerminate();
        }
    }

    HWND IWindowsDesktopWindow::GetNativeHandle() const
    {
        return glfwGetWin32Window(mHandle);
    }

    void IWindowsDesktopWindow::SetSize(float2 size)
    {
        CS_PROFILE_FN();

        glfwSetWindowSize(mHandle, size.x, size.y);
    }

    void IWindowsDesktopWindow::SetPosition(float2 pos)
    {
        CS_PROFILE_FN();

        glfwSetWindowPos(mHandle, pos.x, pos.y);
    }

    void IWindowsDesktopWindow::SetTitle(const String& title)
    {
        CS_PROFILE_FN();

        glfwSetWindowTitle(mHandle, title.c_str());

        EventSystem::DeferEvent<WindowTitleEvent>(title, mData, true);
    }

    void IWindowsDesktopWindow::SetVSync(bool vsync)
    {
        CS_PROFILE_FN();

        if (vsync)
            glfwSwapInterval(1);
        else
            glfwSwapInterval(0);

        mData.IsVSync = vsync;
    }

}

#endif
