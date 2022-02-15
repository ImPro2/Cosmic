module;
#include "cspch.hpp"
#include "WindowsUtils.hpp"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
module Cosmic.Impl.OS.Windows.IWindowsWindow;
CS_MODULE_LOG_INFO(Cosmic, Impl.OS.Windows.IWindowsWindow);

import Cosmic.App.IWindow;
import Cosmic.App.Log;
import Cosmic.Base.Types;
import Cosmic.Base.Tuples;

namespace Cosmic
{

    static uint32 sGLFWWindowCount = 0;

    IWindowsDesktopWindow::IWindowsDesktopWindow(const DesktopWindowInfo& info)
        : IDesktopWindow(info)
    {
        CS_LOG_INFO("Creating window {0}: [{1}:{2}]", mInfo.Title.c_str(), mInfo.Size.width, mInfo.Size.height);

        Init();
        Show();
    }

    IWindowsDesktopWindow::~IWindowsDesktopWindow()
    {
        Close();
    }

    void IWindowsDesktopWindow::Init()
    {
        if (sGLFWWindowCount == 0)
            CS_ASSERT(glfwInit() == GLFW_TRUE, "Failed to initialize GLFW.");

        sGLFWWindowCount++;

        mHandle = glfwCreateWindow((int)mInfo.Size.width, (int)mInfo.Size.height, mInfo.Title.c_str(), nullptr, nullptr);

        // temporary
        {
            glfwMakeContextCurrent(mHandle);
            int32 status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
            CS_ASSERT(status, "Failed to initialize Glad!");
        }

        SetupCallbacks();
    }

    void IWindowsDesktopWindow::SetupCallbacks()
    {
        glfwSetWindowUserPointer(mHandle, &mInfo);

        // TODO: Implement
    }

    void IWindowsDesktopWindow::Show()
    {
        glfwShowWindow(mHandle);
    }

    void IWindowsDesktopWindow::Update()
    {
        glfwPollEvents();
    }

    void IWindowsDesktopWindow::Close()
    {
        glfwDestroyWindow(mHandle);

        sGLFWWindowCount--;

        if (sGLFWWindowCount == 0)
        {
            glfwTerminate();
        }
    }

    void IWindowsDesktopWindow::SetSize(float2 size)
    {
        // TODO:
    }

    void IWindowsDesktopWindow::SetPosition(float2 pos)
    {
        // TODO:
    }

    void IWindowsDesktopWindow::SetTitle(const String& title)
    {
        // TODO:
    }

    void IWindowsDesktopWindow::SetVSync(bool vsync)
    {
        if (vsync)
            glfwSwapInterval(1);
        else
            glfwSwapInterval(0);

        mInfo.IsVSync = vsync;
    }

}