#include "cspch.hpp"
#include "IVulkanDesktopWindow.hpp"

#include "Event/Events.hpp"
#include "Event/Type/WindowEvents.hpp"

#if 0
#include "Impl/RendererAPI/Vulkan/VulkanInstance.hpp"
#include "Impl/RendererAPI/Vulkan/VulkanSurface.hpp"
#include "Impl/RendererAPI/Vulkan/VulkanPhysicalDevice.hpp"
#include "Impl/RendererAPI/Vulkan/VulkanDevice.hpp"
#include "Impl/RendererAPI/Vulkan/VulkanSwapchain.hpp"
#include "Impl/RendererAPI/Vulkan/VulkanQueue.hpp"
#include "Impl/RendererAPI/Vulkan/VulkanSemaphore.hpp"
#endif

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

namespace Cosmic
{

	static bool sGLFWInitialized = false;

	IVulkanDesktopWindow::IVulkanDesktopWindow(const DesktopWindowInfo& info)
		: IDesktopWindow(info)
	{
		Init();
		Show();
	}

	IVulkanDesktopWindow::~IVulkanDesktopWindow()
	{
		Close();
	}

	void IVulkanDesktopWindow::Init()
	{
		if (!sGLFWInitialized)
			glfwInit();

		mHandle = glfwCreateWindow((int32)mData.Size.width, (int32)mData.Size.height, mData.Title.c_str(), nullptr, nullptr);

		SetupCallbacks();

		mGraphicsContext = CreateRef<VulkanGraphicsContext>(mHandle);
		mGraphicsContext->Init();
	}

	void IVulkanDesktopWindow::SetupCallbacks()
	{
		glfwSetWindowUserPointer(mHandle, &mData);

		glfwSetWindowCloseCallback(mHandle, [](GLFWwindow* handle)
		{
			const DesktopWindowInfo& info = *(DesktopWindowInfo*)glfwGetWindowUserPointer(handle);
			EventSystem::DeferEvent<WindowCloseEvent>(info, true);
		});

		glfwSetFramebufferSizeCallback(mHandle, [](GLFWwindow* handle, int32 width, int32 height)
		{
			const DesktopWindowInfo& info = *(DesktopWindowInfo*)glfwGetWindowUserPointer(handle);
			EventSystem::DeferEvent<WindowResizeEvent>(uint2 { (uint32)width, (uint32) height }, info, true);
		});
	}

	void IVulkanDesktopWindow::Show()
	{
		glfwShowWindow(mHandle);
	}

	void IVulkanDesktopWindow::Update()
	{
		mGraphicsContext->Present();
		glfwPollEvents();
	}

	void IVulkanDesktopWindow::Close()
	{
		glfwDestroyWindow(mHandle);
		glfwTerminate();
	}

	HWND IVulkanDesktopWindow::GetNativeHandle() const
	{
		return glfwGetWin32Window(mHandle);
	}

}