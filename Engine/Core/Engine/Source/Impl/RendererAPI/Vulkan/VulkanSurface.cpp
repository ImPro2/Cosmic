#include "cspch.hpp"
#include "VulkanSurface.hpp"
#include "VulkanInstance.hpp"

#ifdef CS_PLATFORM_WINDOWS
#	define GLFW_EXPOSE_NATIVE_WIN32
#	include <GLFW/glfw3native.h>
#	define VK_USE_PLATFORM_WIN32_KHR
#	include <vulkan/vulkan_win32.h>
#elif defined(CS_PLATFORM_LINUX)
#	define GLFW_EXPOSE_NATIVE_X11
#	include <GLFW/glfw3native.h>
#	include <vulkan/vulkan_xlib.h>
#endif

namespace Cosmic
{

#ifdef CS_PLATFORM_WINDOWS
	static void CreateWin32Surface(VkInstance instanceHandle, HWND win32WindowHandle, VkSurfaceKHR& surfaceHandle)
	{
		VkWin32SurfaceCreateInfoKHR win32SurfaceCreateInfo = {
			.sType     = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR,
			.flags     = 0,
			.hinstance = ::GetModuleHandle(nullptr),
			.hwnd      = win32WindowHandle,
		};

		VK_CALL(vkCreateWin32SurfaceKHR(instanceHandle, &win32SurfaceCreateInfo, nullptr, &surfaceHandle));
	}
#endif

	static void CreatePlatformNativeSurface(VkInstance instanceHandle, GLFWwindow* windowHandle, VkSurfaceKHR& surfaceHandle)
	{
#ifdef CS_PLATFORM_WINDOWS
		CreateWin32Surface(instanceHandle, glfwGetWin32Window(windowHandle), surfaceHandle);
#endif
	}

	VulkanSurface::VulkanSurface(const Ref<VulkanInstance>& instance, GLFWwindow* windowHandle)
		: mInstance(instance), mWindowHandle(windowHandle)
	{
		CreatePlatformNativeSurface(instance->GetHandle(), windowHandle, mSurfaceHandle);
	}

	VulkanSurface::~VulkanSurface()
	{
		vkDestroySurfaceKHR(mInstance->GetHandle(), mSurfaceHandle, nullptr);
	}

}