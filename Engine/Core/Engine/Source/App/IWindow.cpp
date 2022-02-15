module;
#include "cspch.hpp"
module Cosmic.App.IWindow;

import Cosmic.Impl.OS.Windows.IWindowsWindow;

namespace Cosmic
{

    Scope<IDesktopWindow> CreateDesktopWindow(const DesktopWindowInfo& info)
    {
#ifdef CS_PLATFORM_WINDOWS
        return CreateScope<IWindowsDesktopWindow>(info);
#endif
    }

    IDesktopWindow::IDesktopWindow(const DesktopWindowInfo& info)
        : mInfo(info)
    {
    }

}