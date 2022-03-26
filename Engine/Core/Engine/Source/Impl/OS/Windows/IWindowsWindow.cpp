module;
#include "cspch.hpp"
#include "WindowsUtils.hpp"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <functional>
module Cosmic.Impl.OS.Windows.IWindowsWindow;
CS_MODULE_LOG_INFO(Cosmic, Impl.OS.Windows.IWindowsWindow);

import Cosmic.App.Log;
import Cosmic.Base.Types;
import Cosmic.Base.Tuples;
import Cosmic.App.IWindow;
import Cosmic.App.WindowInfo;
import Cosmic.App.WindowEvents;

namespace Cosmic
{

    static uint32 sGLFWWindowCount = 0;

    IWindowsDesktopWindow::IWindowsDesktopWindow(const DesktopWindowInfo& info, WindowEventCallback callback)
        : IDesktopWindow(info, callback)
    {
        CS_LOG_INFO("Creating window {0}: [{1}:{2}]", mData.Title.c_str(), mData.Size.width, mData.Size.height);

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

        mHandle = glfwCreateWindow((int)mData.Size.width, (int)mData.Size.height, mData.Title.c_str(), nullptr, nullptr);

        mGraphicsContext = CreateGraphicsContext((void*)mHandle);
        mGraphicsContext->Init();

        SetupCallbacks();

        //xEventSystem::Submit<WindowCreateEvent>(WindowCreateEvent(mData, true));
        mData(WindowCreateEvent(mData.Info, true));
    }

    void IWindowsDesktopWindow::SetupCallbacks()
    {
#define _CS_CALLBACK(event, __VA_ARGS__) info(##event##(__VA_ARGS__))

        glfwSetWindowUserPointer(mHandle, &mData);

        glfwSetWindowCloseCallback(mHandle, [](GLFWwindow* window) {
            DesktopWindowData& data = *(DesktopWindowData*)glfwGetWindowUserPointer(window);
            //xEventSystem::Submit<WindowCloseEvent>(WindowCloseEvent(info, true));
            //xinfo(WindowCloseEvent(info, true));
            //x_CS_CALLBACK(WindowCloseEvent, info, true);
            data(WindowCloseEvent(data.Info, true));
        });

        glfwSetWindowSizeCallback(mHandle, [](GLFWwindow* window, int width, int height) {
            DesktopWindowData& data = *(DesktopWindowData*)glfwGetWindowUserPointer(window);
            data.Size.width  = (uint32)width;
            data.Size.height = (uint32)height;

            //xEventSystem::Submit<WindowResizeEvent>(WindowResizeEvent(info.Size, info, true));
            //x_CS_CALLBACK(WindowResizeEvent, info.Size, info, true);
            data(WindowResizeEvent(data.Size, data.Info, true));
        });

        //xglfwSetWindowMoveCallback(mHandle, [](GLFWwindow* window, int xpos, int ypos) {
        //x    DesktopWindowInfo& info = *(DesktopWindowInfo*)glfwGetWindowUserPointer(window);
        //x    info.Position.x = xpos;
        //x    info.Position.y = ypos;
        //x
        //x    EventSystem::Submit<WindowMoveEvent>(WindowMoveEvent({ xpos, ypos }, info, true));
        //x});

        glfwSetKeyCallback(mHandle, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
            DesktopWindowData& data = *(DesktopWindowData*)glfwGetWindowUserPointer(window);
            static int repeatCount  = 0;

            switch (action)
            {
                case GLFW_REPEAT:
                {
                    repeatCount++;
                    data(KeyPressEvent((uint8)key, repeatCount, data.Info, true));
                    //x_CS_CALLBACK(KeyPressEvent, (uint8)key, repeatCount, info, true);
                    //xEventSystem::Submit<KeyPressEvent>(KeyPressEvent((uint8)key, repeatCount, info, true));
                    break;
                }
                case GLFW_PRESS:
                {
                    data(KeyPressEvent((uint8)key, repeatCount, data.Info, true));
                    //x_CS_CALLBACK(KeyPressEvent, (uint8)key, repeatCount, info, true);
                    //xEventSystem::Submit<KeyPressEvent>(KeyPressEvent((uint8)key, repeatCount, info, true));
                    break;
                }
                case GLFW_RELEASE:
                {
                    data(KeyReleaseEvent((uint8)key, data.Info, true));
                    //x_CS_CALLBACK(KeyReleaseEvent, (uint8)key, info, true);
                    //xEventSystem::Submit<KeyReleaseEvent>(KeyReleaseEvent((uint8)key, info, true));
                    repeatCount = 0;
                    break;
                }
            }
        });

        glfwSetCharCallback(mHandle, [](GLFWwindow* window, unsigned int keycode) {
            DesktopWindowData& data = *(DesktopWindowData*)glfwGetWindowUserPointer(window);
            data(KeyTypeEvent((char)keycode, data.Info, true));
            //x_CS_CALLBACK(KeyTypeEvent, (char)keycode, info, true);
            //xEventSystem::Submit<KeyTypeEvent>(KeyTypeEvent((char)keycode, info, true));
        });

        glfwSetCursorPosCallback(mHandle, [](GLFWwindow* window, double xpos, double ypos) {
            DesktopWindowData& data = *(DesktopWindowData*)glfwGetWindowUserPointer(window);
            data(MouseMoveEvent({ (float32)xpos, (float32)ypos }, data.Info, true));
            //x_CS_CALLBACK(MouseMoveEvent, { (float32)xpos, (float32)ypos }, info, true);
            //xEventSystem::Submit<MouseMoveEvent>(MouseMoveEvent({ (float32)xpos, (float32)ypos }, info, true));
        });

        glfwSetScrollCallback(mHandle, [](GLFWwindow* window, double xoffset, double yoffset) {
            DesktopWindowData& data = *(DesktopWindowData*)glfwGetWindowUserPointer(window);
            data(MouseScrollEvent((float32)yoffset, data.Info, true));
            //x_CS_CALLBACK(MouseScrollEvent, (float32)yoffset, info, true);
            //xEventSystem::Submit<MouseScrollEvent>(MouseScrollEvent((float32)yoffset, info, true));
        });

        glfwSetMouseButtonCallback(mHandle, [](GLFWwindow* window, int button, int action, int mods) {
            DesktopWindowData& data = *(DesktopWindowData*)glfwGetWindowUserPointer(window);

            switch (action)
            {
                case GLFW_PRESS:   //xEventSystem::Submit<MouseButtonClickEvent>(MouseButtonClickEvent((uint8)button, info, true));     break;
                    data(MouseButtonClickEvent((uint8)button, data.Info, true)); break;
                    //x_CS_CALLBACK(MouseButtonClickEvent, (uint8)button, info, true); break;
                case GLFW_RELEASE: //xEventSystem::Submit<MouseButtonReleaseEvent>(MouseButtonReleaseEvent((uint8)button, info, true)); break;
                    data(MouseButtonReleaseEvent((uint8)button, data.Info, true)); break;
                    //x_CS_CALLBACK(MouseButtonReleaseEvent, (uint8)button, info, true); break;
            }
        });
    }

    void IWindowsDesktopWindow::Show()
    {
        glfwShowWindow(mHandle);
    }

    void IWindowsDesktopWindow::Update()
    {
        glfwPollEvents();
        mGraphicsContext->SwapBuffers();
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

        mData(WindowTitleEvent(title, mData.Info, true));
        //xEventSystem::Submit<WindowTitleEvent>(WindowTitleEvent(title, mData, true));
    }

    void IWindowsDesktopWindow::SetVSync(bool vsync)
    {
        if (vsync)
            glfwSwapInterval(1);
        else
            glfwSwapInterval(0);

        mData.IsVSync = vsync;
    }

}