module;
#include "cspch.hpp"
module Cosmic.App.IWindow;

import Cosmic.Impl.OS.Windows.IWindowsWindow;
import Cosmic.App.WindowEvents;

namespace Cosmic
{

    Scope<IDesktopWindow> CreateDesktopWindow(const DesktopWindowInfo& info, WindowEventCallback callback)
    {
        CS_PROFILE_FN();

#ifdef CS_PLATFORM_WINDOWS
        return CreateScope<IWindowsDesktopWindow>(info, callback);
#endif
    }

    IDesktopWindow::IDesktopWindow(const DesktopWindowInfo& info, WindowEventCallback callback)
        : mData(callback, info)
    {
        CS_PROFILE_FN();
    }

}