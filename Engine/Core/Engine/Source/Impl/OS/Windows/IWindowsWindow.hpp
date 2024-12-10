#pragma once
#include "WindowsUtils.hpp"
#include "GLFW/glfw3.h"

#include "App/Window/IWindow.hpp"
#include "App/Window/WindowInfo.hpp"
#include "Event/Type/WindowEvents.hpp"
#include "Renderer/GraphicsContext.hpp"
#include "Base/Types.hpp"
#include "Base/Tuples.hpp"

namespace Cosmic
{

    class IWindowsDesktopWindow : public IDesktopWindow
    {
    public:
        IWindowsDesktopWindow(const DesktopWindowInfo& info);
        virtual ~IWindowsDesktopWindow();

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

        virtual void SetSize(float2 size)          override;
        virtual void SetPosition(float2 pos)       override;
        virtual void SetTitle(const String& title) override;
        virtual void SetVSync(bool vsync)          override;

    private:
        GLFWwindow* mHandle;
    };

}