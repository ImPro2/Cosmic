#include "cspch.hpp"
#include "IVulkanDesktopWindow.hpp"

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
	}

	void IVulkanDesktopWindow::SetupCallbacks()
	{
		glfwSetWindowUserPointer(mHandle, &mCloseCallback);

		glfwSetWindowCloseCallback(mHandle, [](GLFWwindow* handle)
		{
			std::function<void()> closeCallback = *(std::function<void()>*)glfwGetWindowUserPointer(handle);

			closeCallback();
		});
	}

	void IVulkanDesktopWindow::Show()
	{
		glfwShowWindow(mHandle);
	}

	void IVulkanDesktopWindow::Update()
	{
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