#pragma once
#include "Renderer/GraphicsContext.hpp"

#include "VulkanInstance.hpp"
#include "VulkanSurface.hpp"
#include "VulkanPhysicalDevice.hpp"
#include "VulkanDevice.hpp"
#include "VulkanSwapchain.hpp"
#include "VulkanQueue.hpp"
#include "VulkanSemaphore.hpp"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

namespace Cosmic
{

#if 0
	class VulkanInstance;
	class VulkanSurface;
	class VulkanPhysicalDevice;
	class VulkanDevice;
	class VulkanSwapchain;
	class VulkanQueue;
	class VulkanSemaphore;
#endif

	class VulkanGraphicsContext : public GraphicsContext
	{
	public:
		VulkanGraphicsContext(GLFWwindow* windowHandle);
		~VulkanGraphicsContext();

	public:
		virtual void Init()        override;
		virtual void Shutdown()    override;
		virtual void Present()     override;

	public:
		const Ref<VulkanInstance>& GetInstance() const { return mInstance; }
		Ref<VulkanInstance> GetInstance() { return mInstance; }

		const Ref<VulkanSurface>& GetSurface() const { return mSurface; }
		Ref<VulkanSurface>        GetSurface()       { return mSurface; }

		const Ref<VulkanPhysicalDevice>& GetPhysicalDevice() const { return mPhysicalDevice; }
		Ref<VulkanPhysicalDevice>        GetPhysicalDevice()       { return mPhysicalDevice; }

		const Ref<VulkanDevice>& GetDevice() const { return mDevice; }
		Ref<VulkanDevice>        GetDevice()       { return mDevice; }

		const Ref<VulkanSwapchain>& GetSwapchain() const { return mSwapchain; }
		Ref<VulkanSwapchain>        GetSwapchain()       { return mSwapchain; }

		const Ref<VulkanQueue>& GetGraphicsQueue() const { return mGraphicsQueue; }
		Ref<VulkanQueue>        GetGraphicsQueue()       { return mGraphicsQueue; }

		const Ref<VulkanQueue>& GetPresentQueue() const { return mPresentQueue; }
		Ref<VulkanQueue>        GetPresentQueue()       { return mPresentQueue; }

	private:
		GLFWwindow* mWindowHandle;

		Ref<VulkanInstance>       mInstance;
		Ref<VulkanSurface>        mSurface;
		Ref<VulkanPhysicalDevice> mPhysicalDevice;
		Ref<VulkanDevice>         mDevice;
		Ref<VulkanSwapchain>      mSwapchain;

		Ref<VulkanQueue> mGraphicsQueue;
		Ref<VulkanQueue> mPresentQueue;

		Ref<VulkanSemaphore> mPresentSemaphore;
	};

}
