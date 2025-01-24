#include "cspch.hpp"
#include "App/Window/IWindow.hpp"

#ifdef CS_PLATFORM_WINDOWS
#include "Impl/OS/Windows/IWindowsWindow.hpp"
#else if defined(CS_PLATFORM_LINUX)
#include "Impl/OS/Linux/ILinuxWindow.hpp"
#endif

namespace Cosmic
{

    Ref<IDesktopWindow> CreateDesktopWindow(const DesktopWindowInfo& info)
    {
        CS_PROFILE_FN();

#ifdef CS_PLATFORM_WINDOWS
        return CreateRef<IWindowsDesktopWindow>(info);
#else if defined(CS_PLATFORM_LINUX)
        return CreateRef<ILinuxDesktopWindow>(info);
#endif
    }

    IDesktopWindow::IDesktopWindow(const DesktopWindowInfo& info)
        : mData(info)
    {
        CS_PROFILE_FN();
    }

}
