#include "cspch.hpp"
#include "VulkanSwapchain.hpp"

#include "VulkanInternal.hpp"
#include "VulkanDevice.hpp"
#include "VulkanSurface.hpp"
#include "VulkanPhysicalDevice.hpp"
#include "VulkanSemaphore.hpp"
#include "VulkanFence.hpp"

#include <algorithm>

namespace Cosmic
{

	SwapchainSupportDetails VulkanQuerySwapchainSupportDetails(VkSurfaceKHR surfaceHandle, VkPhysicalDevice physicalDeviceHandle)
	{
		SwapchainSupportDetails supportDetails;

		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDeviceHandle, surfaceHandle, &supportDetails.Capabilities);

		uint32 formatCount;
		vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDeviceHandle, surfaceHandle, &formatCount, nullptr);

		if (formatCount > 0)
		{
			supportDetails.Formats.resize(formatCount);
			vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDeviceHandle, surfaceHandle, &formatCount, supportDetails.Formats.data());
		}

		uint32 presentModeCount;
		vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDeviceHandle, surfaceHandle, &presentModeCount, nullptr);

		if (presentModeCount > 0)
		{
			supportDetails.PresentModes.resize(presentModeCount);
			vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDeviceHandle, surfaceHandle, &presentModeCount, supportDetails.PresentModes.data());
		}

		return supportDetails;
	}

	namespace Utils
	{

		static VkSurfaceFormatKHR ChooseSwapchainSurfaceFormat(const Vector<VkSurfaceFormatKHR>& availableFormats)
		{
			for (const VkSurfaceFormatKHR& availableFormat : availableFormats)
			{
				if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
				{
					return availableFormat;
				}
			}

			return availableFormats[0];
		}

		static VkPresentModeKHR ChooseSwapchainPresentMode(const Vector<VkPresentModeKHR>& availablePresentModes)
		{
			for (const VkPresentModeKHR& availablePresentMode : availablePresentModes)
			{
				if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
				{
					return availablePresentMode;
				}
			}

			return VK_PRESENT_MODE_FIFO_KHR;
		}

		static VkExtent2D ChooseSwapchainExtent(VkSurfaceCapabilitiesKHR surfaceCapabilities, GLFWwindow* windowHandle)
		{
			if (surfaceCapabilities.currentExtent.width != std::numeric_limits<uint32>::max())
				return surfaceCapabilities.currentExtent;

			int2 framebufferSize;
			glfwGetFramebufferSize(windowHandle, &framebufferSize.width, &framebufferSize.height);

			VkExtent2D actualExtent = {
				std::clamp((uint32)framebufferSize.width,  surfaceCapabilities.minImageExtent.width,  surfaceCapabilities.minImageExtent.width),
				std::clamp((uint32)framebufferSize.height, surfaceCapabilities.minImageExtent.height, surfaceCapabilities.minImageExtent.height)
			};

			return actualExtent;
		}

	}

	VulkanSwapchain::VulkanSwapchain(GLFWwindow* windowHandle, const Ref<VulkanDevice>& device, const Ref<VulkanSurface>& surface, const Ref<VulkanPhysicalDevice>& physicalDevice)
		: mWindowHandle(windowHandle), mDevice(device), mSurface(surface), mPhysicalDevice(physicalDevice)
	{
		CreateSwapchainHandle();
		CreateImageHandles();
		CreateImageViewHandles();
	}

	VulkanSwapchain::~VulkanSwapchain()
	{
		DestroyHandles();

		vkDestroySwapchainKHR(mDevice->GetHandle(), mSwapchainHandle, nullptr);
	}

	void VulkanSwapchain::Recreate()
	{
		VK_CALL(vkDeviceWaitIdle(mDevice->GetHandle()));

		DestroyHandles();

		CreateSwapchainHandle();
		CreateImageHandles();
		CreateImageViewHandles();
	}

	void VulkanSwapchain::AcquireNextImage(const Ref<VulkanSemaphore>& imageAvailableSemaphore, const Ref<VulkanFence>& fence)
	{
		VkSemaphore semaphoreHandle = imageAvailableSemaphore ? imageAvailableSemaphore->GetHandle() : VK_NULL_HANDLE;
		VkFence     fenceHandle     = fence ? fence->GetHandle() : VK_NULL_HANDLE;

		VkResult acquireNextImageResult = vkAcquireNextImageKHR(mDevice->GetHandle(), mSwapchainHandle, std::numeric_limits<uint64>::max(), semaphoreHandle, fenceHandle, &mImageIndex);

		if (acquireNextImageResult == VK_ERROR_OUT_OF_DATE_KHR || acquireNextImageResult == VK_SUBOPTIMAL_KHR)
		{
			Recreate();
		}
		else if (acquireNextImageResult != VK_SUCCESS)
		{
			VK_ERROR(acquireNextImageResult, "");
		}
	}

	void VulkanSwapchain::CreateSwapchainHandle()
	{
		SwapchainSupportDetails swapchainSupportDetails = VulkanQuerySwapchainSupportDetails(mSurface->GetHandle(), mPhysicalDevice->GetHandle());

		mSurfaceFormat = Utils::ChooseSwapchainSurfaceFormat(swapchainSupportDetails.Formats);
		mPresentMode   = Utils::ChooseSwapchainPresentMode(swapchainSupportDetails.PresentModes);
		mExtent        = Utils::ChooseSwapchainExtent(swapchainSupportDetails.Capabilities, mWindowHandle);

		uint32 imageCount = swapchainSupportDetails.Capabilities.minImageCount + 1;

		if (swapchainSupportDetails.Capabilities.maxImageCount > 0 && imageCount > swapchainSupportDetails.Capabilities.maxImageCount)
			imageCount = swapchainSupportDetails.Capabilities.maxImageCount;

		VkSwapchainCreateInfoKHR swapchainCreateInfo = {
			.sType            = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
			.flags            = 0,
			.surface          = mSurface->GetHandle(),
			.minImageCount    = imageCount,
			.imageFormat      = mSurfaceFormat.format,
			.imageColorSpace  = mSurfaceFormat.colorSpace,
			.imageExtent      = mExtent,
			.imageArrayLayers = 1,
			.imageUsage       = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
			.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE,
			.preTransform     = swapchainSupportDetails.Capabilities.currentTransform,
			.compositeAlpha   = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
			.presentMode      = mPresentMode,
			.clipped          = VK_TRUE,
			.oldSwapchain     = mSwapchainHandle
		};

		VK_CALL(vkCreateSwapchainKHR(mDevice->GetHandle(), & swapchainCreateInfo, nullptr, &mSwapchainHandle));
	}

	void VulkanSwapchain::CreateImageHandles()
	{
		uint32 swapchainImageCount;
		vkGetSwapchainImagesKHR(mDevice->GetHandle(), mSwapchainHandle, &swapchainImageCount, nullptr);

		mImageHandles.clear();
		mImageHandles.resize(swapchainImageCount);

		vkGetSwapchainImagesKHR(mDevice->GetHandle(), mSwapchainHandle, &swapchainImageCount, mImageHandles.data());
	}

	void VulkanSwapchain::CreateImageViewHandles()
	{
		mImageViewHandles.resize(mImageHandles.size());

		for (uint32 i = 0; i < mImageHandles.size(); i++)
		{
			VkImageViewCreateInfo imageViewCreateInfo = {
				.sType      = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
				.flags      = 0,
				.image      = mImageHandles[i],
				.viewType   = VK_IMAGE_VIEW_TYPE_2D,
				.format     = mSurfaceFormat.format,
				.components = {
					.r = VK_COMPONENT_SWIZZLE_IDENTITY,
					.g = VK_COMPONENT_SWIZZLE_IDENTITY,
					.b = VK_COMPONENT_SWIZZLE_IDENTITY,
					.a = VK_COMPONENT_SWIZZLE_IDENTITY
				},
				.subresourceRange = {
					.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT,
					.baseMipLevel   = 0,
					.levelCount     = 1,
					.baseArrayLayer = 0,
					.layerCount     = 1
				}
			};

			VK_CALL(vkCreateImageView(mDevice->GetHandle(), &imageViewCreateInfo, nullptr, &mImageViewHandles[i]));
		}
	}

	void VulkanSwapchain::DestroyHandles()
	{
		for (VkImageView imageViewHandle : mImageViewHandles)
			vkDestroyImageView(mDevice->GetHandle(), imageViewHandle, nullptr);
	}

}