#pragma once
#include "VulkanDebugUtils.hpp"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

namespace Cosmic
{

	class VulkanInstance;

	class VulkanSurface
	{
	public:
		VulkanSurface(const Ref<VulkanInstance>& instance, GLFWwindow* windowHandle);
		~VulkanSurface();

	public:
		VkSurfaceKHR GetHandle()        const { return mSurfaceHandle; }
		GLFWwindow*  GetWindowHandle()  const { return mWindowHandle;  }

	private:
		VkSurfaceKHR mSurfaceHandle;
		GLFWwindow*  mWindowHandle;

		Ref<VulkanInstance> mInstance;
	};

}
