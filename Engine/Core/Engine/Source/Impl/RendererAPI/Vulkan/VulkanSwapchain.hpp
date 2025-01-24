#pragma once
#include "VulkanDebugUtils.hpp"
#include "VulkanStructs.hpp"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

namespace Cosmic
{

	class VulkanDevice;
	class VulkanSurface;
	class VulkanPhysicalDevice;
	class VulkanSemaphore;
	class VulkanFence;

	SwapchainSupportDetails VulkanQuerySwapchainSupportDetails(VkSurfaceKHR surfaceHandle, VkPhysicalDevice physicalDeviceHandle);

	class VulkanSwapchain : public IRefCounted
	{
	public:
		VulkanSwapchain(GLFWwindow* windowHandle, const Ref<VulkanDevice>& device, const Ref<VulkanSurface>& surface, const Ref<VulkanPhysicalDevice>& physicalDevice);

		~VulkanSwapchain();

	public:
		void Recreate();
		void AcquireNextImage(const Ref<VulkanSemaphore>& imageAvailableSemaphore, const Ref<VulkanFence>& waitFence);

	public:
		VkSwapchainKHR* GetHandlePtr()    { return &mSwapchainHandle; }
		VkSwapchainKHR  GetHandle() const { return mSwapchainHandle;  }

		const Vector<VkImage>&     GetImageHandles()     const { return mImageHandles;     }
		const Vector<VkImageView>& GetImageViewHandles() const { return mImageViewHandles; }

		VkSurfaceFormatKHR GetSurfaceFormat() const { return mSurfaceFormat; }
		VkPresentModeKHR   GetPresentMode()   const { return mPresentMode;   }
		VkExtent2D         GetExtent()        const { return mExtent;        }

		uint32 GetImageIndex() const { return mImageIndex; }

	private:
		void CreateSwapchainHandle();
		void CreateImageHandles();
		void CreateImageViewHandles();

		void DestroyHandles();

	private:
		GLFWwindow*         mWindowHandle;

		VkSwapchainKHR      mSwapchainHandle = VK_NULL_HANDLE;
		Vector<VkImage>     mImageHandles;
		Vector<VkImageView> mImageViewHandles;

		VkSurfaceFormatKHR  mSurfaceFormat;
		VkPresentModeKHR    mPresentMode;
		VkExtent2D          mExtent;

		uint32 mImageIndex = 0;

		Ref<VulkanDevice> mDevice;
		Ref<VulkanSurface> mSurface;
		Ref<VulkanPhysicalDevice> mPhysicalDevice;
	};

}
