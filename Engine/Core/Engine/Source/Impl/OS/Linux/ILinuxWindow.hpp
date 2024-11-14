#pragma once

#include <GLFW/glfw3.h>

#include "App/Window/IWindow.hpp"
#include "App/Window/WindowInfo.hpp"
#include "App/Event/WindowEvents.hpp"
#include "Renderer/GraphicsContext.hpp"
#include "Base/Types.hpp"
#include "Base/Tuples.hpp"

namespace Cosmic
{

    class ILinuxDesktopWindow : public IDesktopWindow
    {
    public:
        ILinuxDesktopWindow(const DesktopWindowInfo& info);
        virtual ~ILinuxDesktopWindow();

    protected:
        void Init()   override;
        void SetupCallbacks();

    public:
        void Show()   override;
        void Update() override;
        void Close()  override;

    public:
        WindowHandle GetNativeHandle() const;
        virtual WindowHandle GetHandle() const override { return mHandle; }

        virtual void SetSize(float2 size)          override;
        virtual void SetPosition(float2 pos)       override;
        virtual void SetTitle(const String& title) override;
        virtual void SetVSync(bool vsync)          override;

    private:
        GLFWwindow* mHandle;
    };
    
}
