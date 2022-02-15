module;
#include "cspch.hpp"
#include "WindowsUtils.hpp"
#include "GLFW/glfw3.h"
export module Cosmic.Impl.OS.Windows.IWindowsWindow;

import Cosmic.App.IWindow;
import Cosmic.Base.Types;
import Cosmic.Base.Tuples;

namespace Cosmic
{

    export class IWindowsDesktopWindow : public IDesktopWindow
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
        virtual WindowHandle GetHandle() const override { return mHandle; }

        virtual void SetSize(float2 size)          override;
        virtual void SetPosition(float2 pos)       override;
        virtual void SetTitle(const String& title) override;
        virtual void SetVSync(bool vsync)          override;

    private:
        GLFWwindow* mHandle;
    };

}