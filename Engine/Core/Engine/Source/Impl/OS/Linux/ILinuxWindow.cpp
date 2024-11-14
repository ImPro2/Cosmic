#include "cspch.hpp"

#ifdef CS_PLATFORM_LINUX

#include "ILinuxWindow.hpp"
#define GLFW_EXPOSE_NATIVE_X11
#include <GLFW/glfw3native.h>

CS_MODULE_LOG_INFO(Cosmic, Impl.OS.Linux.ILinuxWindow);

namespace Cosmic {

    static uint32 sGLFWWindowCount = 0;
    
    ILinuxDesktopWindow::ILinuxDesktopWindow(const DesktopWindowInfo &info)
      : IDesktopWindow(info) 
    {
        CS_PROFILE_FN();
        
        CS_LOG_INFO("Creating window {0}: [{1}:{2}]", mData.Title, mData.Size.width, mData.Size.height);
        
        Init();
        Show();
    }
    
    ILinuxDesktopWindow::~ILinuxDesktopWindow()
    {
        CS_PROFILE_FN();

        Close();
    }
    
    void ILinuxDesktopWindow::Init() {
      CS_PROFILE_FN();
    
      if (sGLFWWindowCount == 0)
        CS_ASSERT(glfwInit() == GLFW_TRUE, "Failed to initialize GLFW.");
    
      sGLFWWindowCount++;
    
      mHandle = glfwCreateWindow((int)mData.Size.width, (int)mData.Size.height,
                                 mData.Title.c_str(), nullptr, nullptr);
      mGraphicsContext = CreateGraphicsContext((void *)mHandle);
      mGraphicsContext->Init();
    
      SetupCallbacks();
    
      EventSystem::AddEvent(new WindowCreateEvent(mData, true));
    }
    
    void ILinuxDesktopWindow::SetupCallbacks() {
      CS_PROFILE_FN();
    
      glfwSetWindowUserPointer(mHandle, &mData);
    
      glfwSetWindowCloseCallback(mHandle, [](GLFWwindow *window) {
        DesktopWindowInfo &data =
            *(DesktopWindowInfo *)glfwGetWindowUserPointer(window);
    
        EventSystem::AddEvent(new WindowCloseEvent(data, true));
      });
    
      glfwSetWindowSizeCallback(
          mHandle, [](GLFWwindow *window, int width, int height) {
            DesktopWindowInfo &data =
                *(DesktopWindowInfo *)glfwGetWindowUserPointer(window);
            data.Size.width = (uint32)width;
            data.Size.height = (uint32)height;
    
            EventSystem::AddEvent(new WindowResizeEvent(data.Size, data, true));
          });
    
      glfwSetKeyCallback(mHandle, [](GLFWwindow *window, int key, int scancode,
                                     int action, int mods) {
        DesktopWindowInfo &data =
            *(DesktopWindowInfo *)glfwGetWindowUserPointer(window);
        static int repeatCount = 0;
    
        switch (action) {
        case GLFW_REPEAT: {
          repeatCount++;
          EventSystem::AddEvent(
              new KeyPressEvent((EKeyCode)key, repeatCount, data, true));
          break;
        }
        case GLFW_PRESS: {
          EventSystem::AddEvent(
              new KeyPressEvent((EKeyCode)key, repeatCount, data, true));
          break;
        }
        case GLFW_RELEASE: {
          EventSystem::AddEvent(new KeyReleaseEvent((EKeyCode)key, data, true));
          repeatCount = 0;
          break;
        }
        }
      });
    
      glfwSetCharCallback(mHandle, [](GLFWwindow *window, unsigned int keycode) {
        DesktopWindowInfo &data =
            *(DesktopWindowInfo *)glfwGetWindowUserPointer(window);
        EventSystem::AddEvent(new KeyTypeEvent((char)keycode, data, true));
      });
    
      glfwSetCursorPosCallback(
          mHandle, [](GLFWwindow *window, double xpos, double ypos) {
            DesktopWindowInfo &data =
                *(DesktopWindowInfo *)glfwGetWindowUserPointer(window);
            EventSystem::AddEvent(
                new MouseMoveEvent({(float32)xpos, (float32)ypos}, data, true));
          });
    
      glfwSetScrollCallback(mHandle, [](GLFWwindow *window, double xoffset,
                                        double yoffset) {
        DesktopWindowInfo &data =
            *(DesktopWindowInfo *)glfwGetWindowUserPointer(window);
        EventSystem::AddEvent(new MouseScrollEvent((float32)yoffset, data, true));
      });
    
      glfwSetMouseButtonCallback(
          mHandle, [](GLFWwindow *window, int button, int action, int mods) {
            DesktopWindowInfo &data =
                *(DesktopWindowInfo *)glfwGetWindowUserPointer(window);
    
            switch (action) {
            case GLFW_PRESS:
              EventSystem::AddEvent(
                  new MouseButtonClickEvent((EMouseCode)button, data, true));
            case GLFW_RELEASE:
              EventSystem::AddEvent(
                  new MouseButtonReleaseEvent((EMouseCode)button, data, true));
            }
          });
    }
    
    void ILinuxDesktopWindow::Show() {
      CS_PROFILE_FN();
    
      glfwShowWindow(mHandle);
    }
    
    void ILinuxDesktopWindow::Update() {
      CS_PROFILE_FN();
    
      glfwPollEvents();
      mGraphicsContext->SwapBuffers();
    }
    
    void ILinuxDesktopWindow::Close() {
      CS_PROFILE_FN();
    
      glfwDestroyWindow(mHandle);
    
      sGLFWWindowCount--;
    
      if (sGLFWWindowCount == 0)
        glfwTerminate();
    }
    
    WindowHandle ILinuxDesktopWindow::GetNativeHandle() const {
        glfwGetX11Window(mHandle);
    }
    
    void ILinuxDesktopWindow::SetSize(float2 size) {
      CS_PROFILE_FN();
    
      glfwSetWindowSize(mHandle, size.x, size.y);
    }
    
    void ILinuxDesktopWindow::SetPosition(float2 pos) {
      CS_PROFILE_FN();
    
      glfwSetWindowPos(mHandle, pos.x, pos.y);
    }
    
    void ILinuxDesktopWindow::SetTitle(const String &title) {
      CS_PROFILE_FN();
    
      glfwSetWindowTitle(mHandle, title.c_str());
    
      EventSystem::AddEvent(new WindowTitleEvent(title, mData, true));
    }
    
    void ILinuxDesktopWindow::SetVSync(bool vsync) {
      CS_PROFILE_FN();
    
      if (vsync)
        glfwSwapInterval(1);
      else
        glfwSwapInterval(0);
    
      mData.IsVSync = vsync;
    }

} // namespace Cosmic

#endif
