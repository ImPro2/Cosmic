module;
#include "cspch.hpp"
#include "WindowsUtils.hpp"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
#include <functional>
module Cosmic.Impl.OS.Windows.IWindowsWindow;
CS_MODULE_LOG_INFO(Cosmic, Impl.OS.Windows.IWindowsWindow);

import Cosmic.App.Log;
import Cosmic.Base.Types;
import Cosmic.Base.Tuples;
import Cosmic.App.IWindow;
import Cosmic.App.WindowInfo;
import Cosmic.App.WindowEvents;
import Cosmic.App.KeyAndMouseCodes;

namespace Cosmic
{

    static uint32 sGLFWWindowCount = 0;

    IWindowsDesktopWindow::IWindowsDesktopWindow(const DesktopWindowInfo& info, WindowEventCallback callback)
        : IDesktopWindow(info, callback)
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

        mData(WindowCreateEvent(mData.Info, true));
    }

    void IWindowsDesktopWindow::SetupCallbacks()
    {
        CS_PROFILE_FN();

        glfwSetWindowUserPointer(mHandle, &mData);

        glfwSetWindowCloseCallback(mHandle, [](GLFWwindow* window) {
            DesktopWindowData& data = *(DesktopWindowData*)glfwGetWindowUserPointer(window);

            data(WindowCloseEvent(data.Info, true));
        });

        glfwSetWindowSizeCallback(mHandle, [](GLFWwindow* window, int width, int height) {
            DesktopWindowData& data = *(DesktopWindowData*)glfwGetWindowUserPointer(window);
            data.Size.width  = (uint32)width;
            data.Size.height = (uint32)height;

            data(WindowResizeEvent(data.Size, data.Info, true));
        });

        glfwSetKeyCallback(mHandle, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
            DesktopWindowData& data = *(DesktopWindowData*)glfwGetWindowUserPointer(window);
            static int repeatCount  = 0;

            switch (action)
            {
                case GLFW_REPEAT:
                {
                    repeatCount++;
                    data(KeyPressEvent((EKeyCode)key, repeatCount, data.Info, true));
                    break;
                }
                case GLFW_PRESS:
                {
                    data(KeyPressEvent((EKeyCode)key, repeatCount, data.Info, true));
                    break;
                }
                case GLFW_RELEASE:
                {
                    data(KeyReleaseEvent((EKeyCode)key, data.Info, true));
                    repeatCount = 0;
                    break;
                }
            }
        });

        glfwSetCharCallback(mHandle, [](GLFWwindow* window, unsigned int keycode) {
            DesktopWindowData& data = *(DesktopWindowData*)glfwGetWindowUserPointer(window);
            data(KeyTypeEvent((char)keycode, data.Info, true));
        });

        glfwSetCursorPosCallback(mHandle, [](GLFWwindow* window, double xpos, double ypos) {
            DesktopWindowData& data = *(DesktopWindowData*)glfwGetWindowUserPointer(window);
            data(MouseMoveEvent({ (float32)xpos, (float32)ypos }, data.Info, true));
        });

        glfwSetScrollCallback(mHandle, [](GLFWwindow* window, double xoffset, double yoffset) {
            DesktopWindowData& data = *(DesktopWindowData*)glfwGetWindowUserPointer(window);
            data(MouseScrollEvent((float32)yoffset, data.Info, true));
        });

        glfwSetMouseButtonCallback(mHandle, [](GLFWwindow* window, int button, int action, int mods) {
            DesktopWindowData& data = *(DesktopWindowData*)glfwGetWindowUserPointer(window);

            switch (action)
            {
                case GLFW_PRESS:
                    data(MouseButtonClickEvent((EMouseCode)button, data.Info, true)); break;
                case GLFW_RELEASE:
                    data(MouseButtonReleaseEvent((EMouseCode)button, data.Info, true)); break;
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

        // TODO:
    }

    void IWindowsDesktopWindow::SetPosition(float2 pos)
    {
        CS_PROFILE_FN();

        // TODO:
    }

    void IWindowsDesktopWindow::SetTitle(const String& title)
    {
        CS_PROFILE_FN();

        // TODO:

        mData(WindowTitleEvent(title, mData.Info, true));
        //xEventSystem::Submit<WindowTitleEvent>(WindowTitleEvent(title, mData, true));
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