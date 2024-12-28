#pragma once
#include "App/Window/IWindow.hpp"

#include <GLFW/glfw3.h>
#include <Windows.h>

namespace Cosmic
{

    class IVulkanDesktopWindow : public IDesktopWindow
    {
    public:
        IVulkanDesktopWindow(const DesktopWindowInfo& info);
        virtual ~IVulkanDesktopWindow();

    protected:
        void Init()   override;
        void SetupCallbacks();

    public:
        void Show()   override;
        void Update() override;
        void Close()  override;

    public:
        void SetCloseCallback(std::function<void()> callback) { mCloseCallback = callback; }

    public:
        HWND GetNativeHandle() const;
        virtual WindowHandle GetHandle() const override { return mHandle; }

        virtual void SetSize(float2 size)          override {}
        virtual void SetPosition(float2 pos)       override {}
        virtual void SetTitle(const String& title) override {}
        virtual void SetVSync(bool vsync)          override {}

    private:
        GLFWwindow* mHandle;
        std::function<void()> mCloseCallback;
    };

}
