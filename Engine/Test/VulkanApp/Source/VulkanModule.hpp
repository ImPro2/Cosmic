#pragma once
#include "App/Module.hpp"
#include "Base/Buffer.hpp"

#include "IVulkanDesktopWindow.hpp"

#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

#include <vulkan/vulkan.h>

namespace Cosmic
{

	struct QueueFamilyIndices
	{
		uint32 GraphicsFamily;
		uint32 PresentFamily;

		bool HasGraphicsFamily = true;
		bool HasPresentFamily  = true;

		bool IsComplete() const
		{
			return HasGraphicsFamily && HasPresentFamily;
		}
	};

	struct SwapchainSupportDetails
	{
		VkSurfaceCapabilitiesKHR   Capabilities;
		Vector<VkSurfaceFormatKHR> Formats;
		Vector<VkPresentModeKHR>   PresentModes;

		bool IsAdequate() const
		{
			return !Formats.empty() && !PresentModes.empty();
		}
	};

	class VulkanModule : public IModule
	{
	public:
		void OnInit()        override;
		void OnShutdown()    override;
		void OnUpdate(Dt dt) override;

	private:
		void CreateInstance();
		void SetupDebugMessenger();
		void CreateSurface();
		void PickPhysicalDevice();
		void CreateLogicalDevice();
		void CreateSwapchain();
		void CreateImageViews();
		void CreateRenderPass();
		void CreateGraphicsPipeline();
		void CreateFramebuffers();
		void CreateCommandPool();
		void CreateCommandBuffer();
		void CreateSynchronisationObjects();

	private:
		bool CheckValidationLayerSupport();
		Vector<const char*> GetRequiredExtensions();
		void PopulateVkDebugUtilsMessengerCreateInfoEXT(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
		bool IsDeviceSuitable(VkPhysicalDevice physicalDevice);
		bool CheckDeviceExtensionSupport(VkPhysicalDevice physicalDevice);
		QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice physicalDevice);
		SwapchainSupportDetails QuerySwapchainSupport(VkPhysicalDevice physicalDevice);
		VkSurfaceFormatKHR ChooseSwapchainSurfaceFormat(const Vector<VkSurfaceFormatKHR>& availableFormats);
		VkPresentModeKHR ChooseSwapchainPresentMode(const Vector<VkPresentModeKHR>& availablePresentModes);
		VkExtent2D ChooseSwapchainExtent(const VkSurfaceCapabilitiesKHR& surfaceCapabilities);
		VkShaderModule CreateShaderModule(const Buffer& bytecode);
		void RecordCommandBuffer(VkCommandBuffer commandBuffer, uint32 imageIndex);

	private:
		Scope<IVulkanDesktopWindow> mWindow;

	private:
		const Vector<const char*> mValidationLayers = { "VK_LAYER_KHRONOS_validation"   };
		const Vector<const char*> mDeviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

#ifdef CS_DEBUG
		bool mEnableValidationLayers = true;
#else
		bool mEnableValidationLayers = false;
#endif

	private:
		VkInstance               mVkInstance;
		VkDebugUtilsMessengerEXT mVkDebugUtilsMessengerEXT;
		VkPhysicalDevice         mVkPhysicalDevice;
		VkDevice                 mVkDevice;
		VkQueue                  mVkGraphicsQueue;
		VkQueue                  mVkPresentQueue;
		VkSurfaceKHR             mVkSurfaceKHR;
		VkSwapchainKHR           mVkSwapchainKHR;
		Vector<VkImage>          mVkSwapchainImages;
		Vector<VkImageView>      mVkSwapchainImageViews;
		VkFormat                 mVkSwapchainImageFormat;
		VkExtent2D               mVkSwapchainExtent;
		VkRenderPass             mVkRenderPass;
		VkPipelineLayout         mVkPipelineLayout;
		VkPipeline               mVkGraphicsPipeline;
		Vector<VkFramebuffer>    mVkSwapchainFramebuffers;
		VkCommandPool            mVkCommandPool;
		VkCommandBuffer          mVkCommandBuffer;
		VkSemaphore              mVkImageAvailableSemaphore;
		VkSemaphore              mVkRenderFinishedSemaphore;
		VkFence                  mVkInFlightFence;
	};

}
