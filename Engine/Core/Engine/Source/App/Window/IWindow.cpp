#include "cspch.hpp"
#include "App/Window/IWindow.hpp"
#include "App/Event/WindowEvents.hpp"

#ifdef CS_PLATFORM_WINDOWS
#include "Impl/OS/Windows/IWindowsWindow.hpp"
#endif

namespace Cosmic
{

    Scope<IDesktopWindow> CreateDesktopWindow(const DesktopWindowInfo& info)
    {
        CS_PROFILE_FN();

#ifdef CS_PLATFORM_WINDOWS
        return CreateScope<IWindowsDesktopWindow>(info);
#endif
    }

    IDesktopWindow::IDesktopWindow(const DesktopWindowInfo& info)
        : mData(info)
    {
        CS_PROFILE_FN();
    }

}