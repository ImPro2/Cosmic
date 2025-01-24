#pragma once
#include "App/Window/IWindow.hpp"
#include "Impl/RendererAPI/Vulkan/VulkanGraphicsContext.hpp"

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
        HWND GetNativeHandle() const;
        virtual WindowHandle GetHandle() const override { return mHandle; }

        Ref<VulkanGraphicsContext> GetVulkanGraphicsContext() { return mGraphicsContext; }
        const Ref<VulkanGraphicsContext>& GetVulkanGraphicsContext() const { return mGraphicsContext; }

        virtual void SetSize(float2 size)          override {}
        virtual void SetPosition(float2 pos)       override {}
        virtual void SetTitle(const String& title) override {}
        virtual void SetVSync(bool vsync)          override {}

    private:
        GLFWwindow* mHandle;

        Ref<VulkanGraphicsContext> mGraphicsContext;
    };

}
