#include "cspch.hpp"
#include "VulkanModule.hpp"
#include "VulkanDebugUtils.hpp"

#include "App/Application.hpp"

#include <stb_image.h>
#include <glm/gtc/matrix_transform.hpp>

CS_MODULE_LOG_INFO(VulkanApp, VulkanModule);

#undef CreateWindow

namespace Cosmic
{

	namespace Utils
	{

		const char* VkPhysicalDeviceTypeToString(VkPhysicalDeviceType type)
		{
			switch (type)
			{
				case VK_PHYSICAL_DEVICE_TYPE_OTHER:          return "Other";
				case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU: return "Integrated GPU";
				case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:   return "Discrete GPU";
				case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU:    return "Virtual GPU";
				case VK_PHYSICAL_DEVICE_TYPE_CPU:            return "CPU";
			}

			return "Unknown";
		}

	}

	void VulkanModule::OnInit()
	{
		CreateWindow();
		CreateInstance();
		SetupDebugMessenger();
		CreateSurface();
		PickPhysicalDevice();
		CreateLogicalDevice();
		CreateSwapchain();
		CreateImageViews();
		CreateRenderPass();
		CreateDescriptorSetLayout();
		CreateGraphicsPipeline();
		CreateCommandPool();
		CreateDepthResources();
		CreateFramebuffers();
		CreateTextureImage();
		CreateTextureImageView();
		CreateTextureSampler();
		CreateVertexBuffer();
		CreateIndexBuffer();
		CreateUniformBuffers();
		CreateDescriptorPool();
		CreateDescriptorSets();
		CreateCommandBuffers();
		CreateSynchronisationObjects();
	}

	void VulkanModule::OnShutdown()
	{
		vkDeviceWaitIdle(mVkDevice);

		for (size_t i = 0; i < sMaxFramesInFlight; i++)
		{
			vkDestroySemaphore(mVkDevice, mVkImageAvailableSemaphores[i], nullptr);
			vkDestroySemaphore(mVkDevice, mVkRenderFinishedSemaphores[i], nullptr);
			vkDestroyFence(mVkDevice, mVkInFlightFences[i], nullptr);
		}

		CleanupSwapchain();

		vkDestroySampler(mVkDevice, mVkTextureSampler, nullptr);
		vkDestroyImageView(mVkDevice, mVkTextureImageView, nullptr);
		vkDestroyImage(mVkDevice, mVkTextureImage, nullptr);
		vkFreeMemory(mVkDevice, mVkTextureImageMemory, nullptr);

		for (size_t i = 0; i < sMaxFramesInFlight; i++)
		{
			vkDestroyBuffer(mVkDevice, mVkUniformBuffers[i], nullptr);
			vkFreeMemory(mVkDevice, mVkUniformBuffersMemory[i], nullptr);
		}

		vkDestroyDescriptorSetLayout(mVkDevice, mVkDescriptorSetLayout, nullptr);
		vkDestroyDescriptorPool(mVkDevice, mVkDescriptorPool, nullptr);

		vkDestroyBuffer(mVkDevice, mVkVertexBuffer, nullptr);
		vkFreeMemory(mVkDevice, mVkVertexBufferMemory, nullptr);

		vkDestroyBuffer(mVkDevice, mVkIndexBuffer, nullptr);
		vkFreeMemory(mVkDevice, mVkIndexBufferMemory, nullptr);

		vkFreeCommandBuffers(mVkDevice, mVkCommandPool, mVkCommandBuffers.size(), mVkCommandBuffers.data());
		vkDestroyCommandPool(mVkDevice, mVkCommandPool, nullptr);

		vkDestroyPipeline(mVkDevice, mVkGraphicsPipeline, nullptr);
		vkDestroyPipelineLayout(mVkDevice, mVkPipelineLayout, nullptr);
		vkDestroyRenderPass(mVkDevice, mVkRenderPass, nullptr);

		vkDestroySurfaceKHR(mVkInstance, mVkSurfaceKHR, nullptr);
		vkDestroyDevice(mVkDevice, nullptr);

		if (mEnableValidationLayers)
		{
			VulkanDestroyDebugUtilsMessengerEXT(mVkInstance, mVkDebugUtilsMessengerEXT, nullptr);
		}

		vkDestroyInstance(mVkInstance, nullptr);

		mVkPhysicalDevice = VK_NULL_HANDLE;
	}

	void VulkanModule::OnUpdate(Dt dt)
	{
		// Acquire image from the swapchain
		// Record command buffer which draws the scene onto that image
		// Submit the recorded command buffer
		// Present swapchain image

		VK_CALL(vkWaitForFences(mVkDevice, 1, &mVkInFlightFences[mCurrentFrameIndex], VK_TRUE, std::numeric_limits<uint64>::max()));

		uint32 imageIndex;

		VkResult acquireNextImageResult = vkAcquireNextImageKHR(mVkDevice, mVkSwapchainKHR, std::numeric_limits<uint64>::max(), mVkImageAvailableSemaphores[mCurrentFrameIndex], VK_NULL_HANDLE, &imageIndex);

		if (acquireNextImageResult == VK_ERROR_OUT_OF_DATE_KHR || acquireNextImageResult == VK_SUBOPTIMAL_KHR || mFramebufferResized)
		{
			RecreateSwapchain();

			mFramebufferResized = false;
			return;
		}

		VK_CALL(vkResetFences(mVkDevice, 1, &mVkInFlightFences[mCurrentFrameIndex]));
		VK_CALL(vkResetCommandBuffer(mVkCommandBuffers[mCurrentFrameIndex], 0));

		UpdateUniformBuffer(mCurrentFrameIndex);
		RecordCommandBuffer(mVkCommandBuffers[mCurrentFrameIndex], imageIndex);

		VkSemaphore          waitSemaphores[]   = { mVkImageAvailableSemaphores[mCurrentFrameIndex] };
		VkSemaphore          signalSemaphores[] = { mVkRenderFinishedSemaphores[mCurrentFrameIndex] };
		VkPipelineStageFlags waitStages[]       = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT   };

		VkSubmitInfo submitInfo         = {};
		submitInfo.sType                = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.waitSemaphoreCount   = 1;
		submitInfo.pWaitSemaphores      = waitSemaphores;
		submitInfo.pWaitDstStageMask    = waitStages;
		submitInfo.commandBufferCount   = 1;
		submitInfo.pCommandBuffers      = &mVkCommandBuffers[mCurrentFrameIndex];
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores    = signalSemaphores;

		VkResult queueSubmitResult = vkQueueSubmit(mVkGraphicsQueue, 1, &submitInfo, mVkInFlightFences[mCurrentFrameIndex]);

		if (queueSubmitResult == VK_ERROR_OUT_OF_DATE_KHR || queueSubmitResult == VK_SUBOPTIMAL_KHR || mFramebufferResized)
		{
			RecreateSwapchain();
			mFramebufferResized = false;

			return;
		}

		VkPresentInfoKHR presentInfo   = {};
		presentInfo.sType              = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores    = signalSemaphores;
		presentInfo.swapchainCount     = 1;
		presentInfo.pSwapchains        = &mVkSwapchainKHR;
		presentInfo.pImageIndices      = &imageIndex;
		presentInfo.pResults           = nullptr;

		VK_CALL(vkQueuePresentKHR(mVkPresentQueue, &presentInfo));

		mWindow->Update();

		mCurrentFrameIndex = (mCurrentFrameIndex + 1) % sMaxFramesInFlight;

		static TimeUnit lastTime = Time::GetTime();
		if (Time::GetTime() - lastTime > 1.0f)
		{
			lastTime = Time::GetTime();
			CS_LOG_TRACE("dt: {}ms, fps: {}", Time::GetDeltaTime().InMilliSeconds(), Time::GetFPS().InSeconds());
		}
	}

	void VulkanModule::OnEvent(const IEvent& e)
	{
		EventDispatcher dispatcher(e);
		CS_DISPATCH_EVENT(WindowResizeEvent, OnWindowResize);
	}

	bool VulkanModule::OnWindowResize(const WindowResizeEvent& e)
	{
		mFramebufferResized = true;
		return false;
	}

	void VulkanModule::CreateWindow()
	{
		DesktopWindowInfo info;
		info.Title = "Vulkan App";

		mWindow = CreateScope<IVulkanDesktopWindow>(info);
	}

	void VulkanModule::CreateInstance()
	{
		if (mEnableValidationLayers)
		{
			CS_ASSERT(CheckValidationLayerSupport(), "Vulkan Error: Required Validation Layers Unavailable.");
		}

		const ApplicationInfo& info = Application::Get()->GetInfo();

		VkApplicationInfo appInfo  = {};
		appInfo.sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName   = info.Name;
		appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.pEngineName        = "Cosmic Engine";
		appInfo.engineVersion      = VK_MAKE_VERSION(1, 0, 0);
		appInfo.apiVersion         = VK_API_VERSION_1_0;

		Vector<const char*> extensions = GetRequiredExtensions();

		VkInstanceCreateInfo createInfo    = {};
		createInfo.sType                   = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo        = &appInfo;
		createInfo.enabledExtensionCount   = extensions.size();
		createInfo.ppEnabledExtensionNames = extensions.data();
		createInfo.enabledLayerCount       = mEnableValidationLayers ? mValidationLayers.size() : 0;
		createInfo.ppEnabledLayerNames     = mEnableValidationLayers ? mValidationLayers.data() : nullptr;

		VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo = {};

		if (mEnableValidationLayers)
		{
			PopulateVkDebugUtilsMessengerCreateInfoEXT(debugCreateInfo);
			createInfo.pNext = &debugCreateInfo;
		}

		VK_CALL(vkCreateInstance(&createInfo, nullptr, &mVkInstance));
	}


	void VulkanModule::SetupDebugMessenger()
	{
		if (!mEnableValidationLayers)
			return;

		VkDebugUtilsMessengerCreateInfoEXT createInfo = {};
		PopulateVkDebugUtilsMessengerCreateInfoEXT(createInfo);
		
		VK_CALL(VulkanCreateDebugUtilsMessengerEXT(mVkInstance, &createInfo, nullptr, &mVkDebugUtilsMessengerEXT));
	}

	void VulkanModule::CreateSurface()
	{
		VkWin32SurfaceCreateInfoKHR createInfo = {};
		createInfo.sType                       = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
		createInfo.hwnd                        = (HWND)mWindow->GetNativeHandle();
		createInfo.hinstance                   = ::GetModuleHandle(nullptr);

		VK_CALL(vkCreateWin32SurfaceKHR(mVkInstance, &createInfo, nullptr, &mVkSurfaceKHR));
	}

	void VulkanModule::PickPhysicalDevice()
	{
		uint32 deviceCount = 0;
		VK_CALL(vkEnumeratePhysicalDevices(mVkInstance, &deviceCount, nullptr));

		CS_ASSERT(deviceCount != 0, "No GPU Physical Device Found");

		VkPhysicalDevice* physicalDevices = (VkPhysicalDevice*)alloca(deviceCount * sizeof(VkPhysicalDevice));
		VK_CALL(vkEnumeratePhysicalDevices(mVkInstance, &deviceCount, physicalDevices));

		int32 suitablePhysicalDeviceIndex = -1;

		for (int32 i = 0; i < deviceCount; i++)
		{
			if (IsDeviceSuitable(physicalDevices[i]))
			{
				suitablePhysicalDeviceIndex = i;
				break;
			}
		}

		if (suitablePhysicalDeviceIndex == -1)
		{
			CS_LOG_WARN("No suitable device found");
			suitablePhysicalDeviceIndex = 0;
		}

		mVkPhysicalDevice = physicalDevices[suitablePhysicalDeviceIndex];

		// Log device properties

		if (mEnableValidationLayers)
		{
			VkPhysicalDeviceProperties properties;
			vkGetPhysicalDeviceProperties(mVkPhysicalDevice, &properties);

			const char* deviceTypeStr = Utils::VkPhysicalDeviceTypeToString(properties.deviceType);

			CS_LOG_DEBUG("Vulkan Physical Device Information:");
			CS_LOG_DEBUG("\tAPI Version:   {}", properties.apiVersion);
			CS_LOG_DEBUG("\tDriver Version {}", properties.driverVersion);
			CS_LOG_DEBUG("\tVendor ID:     {}", properties.vendorID);
			CS_LOG_DEBUG("\tDevice ID:     {}", properties.deviceID);
			CS_LOG_DEBUG("\tDevice Type:   {}", deviceTypeStr);
			CS_LOG_DEBUG("\tDevice Name:   {}", properties.deviceName);
		}
	}

	void VulkanModule::CreateLogicalDevice()
	{
		QueueFamilyIndices indices = FindQueueFamilies(mVkPhysicalDevice);

		Set<uint32> uniqueQueueFamilies;

		if (indices.HasGraphicsFamily) uniqueQueueFamilies.insert(indices.GraphicsFamily);
		if (indices.HasPresentFamily)  uniqueQueueFamilies.insert(indices.PresentFamily);

		VkDeviceQueueCreateInfo* queueCreateInfos = (VkDeviceQueueCreateInfo*)alloca(uniqueQueueFamilies.size() * sizeof(VkDeviceQueueCreateInfo));

		float32 queuePriority = 1.0f;
		uint32  i             = 0;

		for (uint32 queueFamilyIndex : uniqueQueueFamilies)
		{
			VkDeviceQueueCreateInfo queueCreateInfo = {};
			queueCreateInfo.sType                   = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queueCreateInfo.queueFamilyIndex        = queueFamilyIndex;
			queueCreateInfo.queueCount              = 1;
			queueCreateInfo.pQueuePriorities        = &queuePriority;

			queueCreateInfos[i++] = queueCreateInfo;
		}

		VkPhysicalDeviceFeatures deviceFeatures = {};
		deviceFeatures.samplerAnisotropy = VK_TRUE;

		VkDeviceCreateInfo createInfo      = {};
		createInfo.sType                   = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		createInfo.pQueueCreateInfos       = queueCreateInfos;
		createInfo.queueCreateInfoCount    = uniqueQueueFamilies.size();
		createInfo.pEnabledFeatures        = &deviceFeatures;
		createInfo.enabledExtensionCount   = mDeviceExtensions.size();
		createInfo.ppEnabledExtensionNames = mDeviceExtensions.data();

		// Legacy (unnecessary for modern implementations)

		createInfo.enabledLayerCount       = mEnableValidationLayers ? mValidationLayers.size() : 0;
		createInfo.ppEnabledLayerNames     = mEnableValidationLayers ? mValidationLayers.data() : nullptr;

		VK_CALL(vkCreateDevice(mVkPhysicalDevice, &createInfo, nullptr, &mVkDevice));

		vkGetDeviceQueue(mVkDevice, indices.GraphicsFamily, 0, &mVkGraphicsQueue);
		vkGetDeviceQueue(mVkDevice, indices.PresentFamily,  0, &mVkPresentQueue);
	}

	void VulkanModule::CreateSwapchain()
	{
		SwapchainSupportDetails supportDetails = QuerySwapchainSupport(mVkPhysicalDevice);

		VkSurfaceFormatKHR surfaceFormat = ChooseSwapchainSurfaceFormat(supportDetails.Formats);
		VkPresentModeKHR   presentMode   = ChooseSwapchainPresentMode(supportDetails.PresentModes);
		VkExtent2D         extent        = ChooseSwapchainExtent(supportDetails.Capabilities);

		uint32 imageCount = supportDetails.Capabilities.minImageCount + 1;

		if (supportDetails.Capabilities.maxImageCount > 0 && imageCount > supportDetails.Capabilities.maxImageCount)
			imageCount = supportDetails.Capabilities.maxImageCount;

		VkSwapchainCreateInfoKHR createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		createInfo.surface = mVkSurfaceKHR;
		createInfo.minImageCount = imageCount;
		createInfo.imageFormat = surfaceFormat.format;
		createInfo.imageColorSpace = surfaceFormat.colorSpace;
		createInfo.imageExtent = extent;
		createInfo.imageArrayLayers = 1;
		createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
		createInfo.preTransform = supportDetails.Capabilities.currentTransform;
		createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		createInfo.presentMode = presentMode;
		createInfo.clipped = VK_TRUE;
		createInfo.oldSwapchain = VK_NULL_HANDLE;

		QueueFamilyIndices indices = FindQueueFamilies(mVkPhysicalDevice);
		uint32 queueFamilyIndices[] = { indices.GraphicsFamily, indices.PresentFamily };

		if (indices.GraphicsFamily != indices.PresentFamily)
		{
			createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
			createInfo.queueFamilyIndexCount = 2;
			createInfo.pQueueFamilyIndices = queueFamilyIndices;
		}
		else
		{
			createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		}

		VK_CALL(vkCreateSwapchainKHR(mVkDevice, &createInfo, nullptr, &mVkSwapchainKHR));

		uint32 swapchainImageCount;
		vkGetSwapchainImagesKHR(mVkDevice, mVkSwapchainKHR, &swapchainImageCount, nullptr);

		mVkSwapchainImages.resize(swapchainImageCount);
		vkGetSwapchainImagesKHR(mVkDevice, mVkSwapchainKHR, &swapchainImageCount, mVkSwapchainImages.data());

		mVkSwapchainImageFormat = surfaceFormat.format;
		mVkSwapchainExtent      = extent;
	}

	void VulkanModule::CreateImageViews()
	{
		mVkSwapchainImageViews.resize(mVkSwapchainImages.size());

		for (uint32 i = 0; i < mVkSwapchainImages.size(); i++)
		{
			mVkSwapchainImageViews[i] = CreateImageView(mVkSwapchainImages[i], mVkSwapchainImageFormat, VK_IMAGE_ASPECT_COLOR_BIT, 1);
		}
	}

	void VulkanModule::CreateRenderPass()
	{
		VkAttachmentDescription colorAttachment = {};
		colorAttachment.format                  = mVkSwapchainImageFormat;
		colorAttachment.samples                 = VK_SAMPLE_COUNT_1_BIT;
		colorAttachment.loadOp                  = VK_ATTACHMENT_LOAD_OP_CLEAR;
		colorAttachment.storeOp                 = VK_ATTACHMENT_STORE_OP_STORE;
		colorAttachment.stencilLoadOp           = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		colorAttachment.stencilStoreOp          = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		colorAttachment.initialLayout           = VK_IMAGE_LAYOUT_UNDEFINED;
		colorAttachment.finalLayout             = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

		VkAttachmentDescription depthStencilAttachment = {};
		depthStencilAttachment.format                  = FindDepthFormat();
		depthStencilAttachment.samples                 = VK_SAMPLE_COUNT_1_BIT;
		depthStencilAttachment.loadOp                  = VK_ATTACHMENT_LOAD_OP_CLEAR;
		depthStencilAttachment.storeOp                 = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		depthStencilAttachment.stencilLoadOp           = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		depthStencilAttachment.stencilStoreOp          = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		depthStencilAttachment.initialLayout           = VK_IMAGE_LAYOUT_UNDEFINED;
		depthStencilAttachment.finalLayout             = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

		VkAttachmentDescription attachments[] = { colorAttachment, depthStencilAttachment };

		VkAttachmentReference colorAttachmentRef = {};
		colorAttachmentRef.attachment            = 0;
		colorAttachmentRef.layout                = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		VkAttachmentReference depthStencilAttachmentRef = {};
		depthStencilAttachmentRef.attachment            = 1;
		depthStencilAttachmentRef.layout                = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

		VkSubpassDescription subpass    = {};
		subpass.pipelineBindPoint       = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpass.colorAttachmentCount    = 1;
		subpass.pColorAttachments       = &colorAttachmentRef;
		subpass.pDepthStencilAttachment = &depthStencilAttachmentRef;

		VkSubpassDependency dependency = {};
		dependency.srcSubpass          = VK_SUBPASS_EXTERNAL;
		dependency.dstSubpass          = 0;
		dependency.srcStageMask        = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
		dependency.srcAccessMask       = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
		dependency.dstStageMask        = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
		dependency.dstAccessMask       = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

		VkRenderPassCreateInfo createInfo = {};
		createInfo.sType                  = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		createInfo.attachmentCount        = sizeof(attachments) / sizeof(attachments[0]);
		createInfo.pAttachments           = attachments;
		createInfo.subpassCount           = 1;
		createInfo.pSubpasses             = &subpass;
		createInfo.dependencyCount        = 1;
		createInfo.pDependencies          = &dependency;

		VK_CALL(vkCreateRenderPass(mVkDevice, &createInfo, nullptr, &mVkRenderPass));
	}

	void VulkanModule::CreateDescriptorSetLayout()
	{
		VkDescriptorSetLayoutBinding uboLayoutBinding = {};
		uboLayoutBinding.binding                      = 0;
		uboLayoutBinding.descriptorCount              = 1;
		uboLayoutBinding.descriptorType               = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		uboLayoutBinding.stageFlags                   = VK_SHADER_STAGE_VERTEX_BIT;
		uboLayoutBinding.pImmutableSamplers           = nullptr;

		VkDescriptorSetLayoutBinding samplerLayoutBinding = {};
		samplerLayoutBinding.binding                      = 1;
		samplerLayoutBinding.descriptorCount              = 1;
		samplerLayoutBinding.descriptorType               = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		samplerLayoutBinding.stageFlags                   = VK_SHADER_STAGE_FRAGMENT_BIT;
		samplerLayoutBinding.pImmutableSamplers           = nullptr;

		VkDescriptorSetLayoutBinding layoutBindings[] = { uboLayoutBinding, samplerLayoutBinding };

		VkDescriptorSetLayoutCreateInfo createInfo = {};
		createInfo.sType                           = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		createInfo.bindingCount                    = sizeof(layoutBindings) / sizeof(layoutBindings[0]);
		createInfo.pBindings                       = layoutBindings;

		VK_CALL(vkCreateDescriptorSetLayout(mVkDevice, &createInfo, nullptr, &mVkDescriptorSetLayout));
	}

	void VulkanModule::CreateGraphicsPipeline()
	{
		File vertexShaderBytecodeFile   = FileSystem::GetCurrentWorkingDirectory() / "Engine/Test/VulkanApp/Assets/Shaders/VertexShader.spv";
		File fragmentShaderBytecodeFile = FileSystem::GetCurrentWorkingDirectory() / "Engine/Test/VulkanApp/Assets/Shaders/FragmentShader.spv";

		const Buffer vertexShaderBytecode   = vertexShaderBytecodeFile.ReadBinary();
		const Buffer fragmentShaderBytecode = fragmentShaderBytecodeFile.ReadBinary();

		VkShaderModule vertexShaderModule   = CreateShaderModule(vertexShaderBytecode);
		VkShaderModule fragmentShaderModule = CreateShaderModule(fragmentShaderBytecode);

		VkPipelineShaderStageCreateInfo vertexShaderStageInfo = {};
		vertexShaderStageInfo.sType                           = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		vertexShaderStageInfo.stage                           = VK_SHADER_STAGE_VERTEX_BIT;
		vertexShaderStageInfo.module                          = vertexShaderModule;
		vertexShaderStageInfo.pName                           = "main";

		VkPipelineShaderStageCreateInfo fragmentShaderStageInfo = {};
		fragmentShaderStageInfo.sType                           = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		fragmentShaderStageInfo.stage                           = VK_SHADER_STAGE_FRAGMENT_BIT;
		fragmentShaderStageInfo.module                          = fragmentShaderModule;
		fragmentShaderStageInfo.pName                           = "main";

		VkPipelineShaderStageCreateInfo shaderStages[] = { vertexShaderStageInfo, fragmentShaderStageInfo };

		VkDynamicState dynamicStates[] = {
			VK_DYNAMIC_STATE_VIEWPORT,
			VK_DYNAMIC_STATE_SCISSOR
		};

		VkPipelineDynamicStateCreateInfo dynamicStateInfo = {};
		dynamicStateInfo.sType                            = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
		dynamicStateInfo.dynamicStateCount                = sizeof(dynamicStates) / sizeof(dynamicStates[0]);
		dynamicStateInfo.pDynamicStates                   = dynamicStates;

		VkVertexInputBindingDescription           vertexBindingDescription     = Vertex::GetVkVertexInputBindingDescription();
		Vector<VkVertexInputAttributeDescription> vertexAttributesDescriptions = Vertex::GetVkVertexInputAttributeDescriptions();

		VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
		vertexInputInfo.sType                                = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		vertexInputInfo.vertexBindingDescriptionCount        = 1;
		vertexInputInfo.pVertexBindingDescriptions           = &vertexBindingDescription;
		vertexInputInfo.vertexAttributeDescriptionCount      = vertexAttributesDescriptions.size();
		vertexInputInfo.pVertexAttributeDescriptions         = vertexAttributesDescriptions.data();

		VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo = {};
		inputAssemblyInfo.sType                                  = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		inputAssemblyInfo.topology                               = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		inputAssemblyInfo.primitiveRestartEnable                 = VK_FALSE;
		
		VkViewport viewport = {};
		viewport.x          = 0.0f;
		viewport.y          = 0.0f;
		viewport.width      = (float32)mVkSwapchainExtent.width;
		viewport.height     = (float32)mVkSwapchainExtent.height;
		viewport.minDepth   = 0.0f;
		viewport.maxDepth   = 1.0f;

		VkRect2D scissorRect = {};
		scissorRect.offset   = { 0, 0 };
		scissorRect.extent   = mVkSwapchainExtent;

		VkPipelineViewportStateCreateInfo viewportStateInfo = {};
		viewportStateInfo.sType                             = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		viewportStateInfo.viewportCount                     = 1;
		viewportStateInfo.pViewports                        = &viewport;
		viewportStateInfo.scissorCount                      = 1;
		viewportStateInfo.pScissors                         = &scissorRect;

		VkPipelineRasterizationStateCreateInfo rasterizationInfo = {};
		rasterizationInfo.sType                                  = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		rasterizationInfo.depthClampEnable                       = VK_FALSE;
		rasterizationInfo.rasterizerDiscardEnable                = VK_FALSE;
		rasterizationInfo.polygonMode                            = VK_POLYGON_MODE_FILL;
		rasterizationInfo.lineWidth                              = 1.0f;
		rasterizationInfo.cullMode                               = VK_CULL_MODE_BACK_BIT;
		rasterizationInfo.frontFace                              = VK_FRONT_FACE_COUNTER_CLOCKWISE;
		rasterizationInfo.depthBiasEnable                        = VK_FALSE;

		VkPipelineMultisampleStateCreateInfo multisamplingInfo = {};
		multisamplingInfo.sType                                = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		multisamplingInfo.sampleShadingEnable                  = VK_FALSE;
		multisamplingInfo.rasterizationSamples                 = VK_SAMPLE_COUNT_1_BIT;
		multisamplingInfo.minSampleShading                     = 1.0f;
		multisamplingInfo.pSampleMask                          = nullptr;
		multisamplingInfo.alphaToCoverageEnable                = VK_FALSE;
		multisamplingInfo.alphaToOneEnable                     = VK_FALSE;

		VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
		colorBlendAttachment.colorWriteMask                      = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
		colorBlendAttachment.blendEnable                         = VK_TRUE;
		colorBlendAttachment.srcColorBlendFactor                 = VK_BLEND_FACTOR_SRC_ALPHA;
		colorBlendAttachment.dstColorBlendFactor                 = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
		colorBlendAttachment.colorBlendOp                        = VK_BLEND_OP_ADD;
		colorBlendAttachment.srcAlphaBlendFactor                 = VK_BLEND_FACTOR_ONE;
		colorBlendAttachment.dstAlphaBlendFactor                 = VK_BLEND_FACTOR_ZERO;
		colorBlendAttachment.alphaBlendOp                        = VK_BLEND_OP_ADD;

		VkPipelineColorBlendStateCreateInfo colorBlendInfo = {};
		colorBlendInfo.sType                               = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		colorBlendInfo.logicOpEnable                       = VK_FALSE;
		colorBlendInfo.attachmentCount                     = 1;
		colorBlendInfo.pAttachments                        = &colorBlendAttachment;

		VkPipelineDepthStencilStateCreateInfo depthStencilInfo = {};
		depthStencilInfo.sType                                 = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
		depthStencilInfo.depthTestEnable                       = VK_TRUE;
		depthStencilInfo.depthWriteEnable                      = VK_TRUE;
		depthStencilInfo.depthCompareOp                        = VK_COMPARE_OP_LESS;
		depthStencilInfo.depthBoundsTestEnable                 = VK_FALSE;
		depthStencilInfo.minDepthBounds                        = 0.0f;
		depthStencilInfo.maxDepthBounds                        = 1.0f;
		depthStencilInfo.stencilTestEnable                     = VK_FALSE;
		depthStencilInfo.front                                 = {};
		depthStencilInfo.back                                  = {};

		VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
		pipelineLayoutInfo.sType                      = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount             = 1;
		pipelineLayoutInfo.pSetLayouts                = &mVkDescriptorSetLayout;
		pipelineLayoutInfo.pushConstantRangeCount     = 0;
		pipelineLayoutInfo.pPushConstantRanges        = nullptr;

		VK_CALL(vkCreatePipelineLayout(mVkDevice, &pipelineLayoutInfo, nullptr, &mVkPipelineLayout));

		VkGraphicsPipelineCreateInfo createInfo = {};
		createInfo.sType                        = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		createInfo.stageCount                   = sizeof(shaderStages) / sizeof(shaderStages[0]);
		createInfo.pStages                      = shaderStages;
		createInfo.pVertexInputState            = &vertexInputInfo;
		createInfo.pInputAssemblyState          = &inputAssemblyInfo;
		createInfo.pViewportState               = &viewportStateInfo;
		createInfo.pRasterizationState          = &rasterizationInfo;
		createInfo.pMultisampleState            = &multisamplingInfo;
		createInfo.pDepthStencilState           = &depthStencilInfo;
		createInfo.pColorBlendState             = &colorBlendInfo;
		createInfo.pDynamicState                = &dynamicStateInfo;
		createInfo.layout                       = mVkPipelineLayout;
		createInfo.renderPass                   = mVkRenderPass;
		createInfo.subpass                      = 0;
		createInfo.basePipelineHandle           = VK_NULL_HANDLE;
		createInfo.basePipelineIndex            = -1;

		VK_CALL(vkCreateGraphicsPipelines(mVkDevice, VK_NULL_HANDLE, 1, &createInfo, nullptr, &mVkGraphicsPipeline));

		vkDestroyShaderModule(mVkDevice, vertexShaderModule,   nullptr);
		vkDestroyShaderModule(mVkDevice, fragmentShaderModule, nullptr);
	}

	void VulkanModule::CreateCommandPool()
	{
		QueueFamilyIndices queueFamilyIndices = FindQueueFamilies(mVkPhysicalDevice);

		VkCommandPoolCreateInfo createInfo = {};
		createInfo.sType                   = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		createInfo.flags                   = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		createInfo.queueFamilyIndex        = queueFamilyIndices.GraphicsFamily;

		VK_CALL(vkCreateCommandPool(mVkDevice, &createInfo, nullptr, &mVkCommandPool));
	}

	void VulkanModule::CreateDepthResources()
	{
		VkFormat depthFormat = FindDepthFormat();

		CreateImage({ mVkSwapchainExtent.width, mVkSwapchainExtent.height }, 1, depthFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, mVkDepthImage, mVkDepthImageMemory);
		mVkDepthImageView = CreateImageView(mVkDepthImage, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT, 1);

	}

	void VulkanModule::CreateFramebuffers()
	{
		mVkSwapchainFramebuffers.resize(mVkSwapchainImageViews.size());

		for (size_t i = 0; i < mVkSwapchainImageViews.size(); i++)
		{
			VkImageView attachments[] = { mVkSwapchainImageViews[i], mVkDepthImageView };

			VkFramebufferCreateInfo createInfo = {};
			createInfo.sType                   = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			createInfo.renderPass              = mVkRenderPass;
			createInfo.attachmentCount         = sizeof(attachments) / sizeof(attachments[0]);
			createInfo.pAttachments            = attachments;
			createInfo.width                   = mVkSwapchainExtent.width;
			createInfo.height                  = mVkSwapchainExtent.height;
			createInfo.layers                  = 1;

			VK_CALL(vkCreateFramebuffer(mVkDevice, &createInfo, nullptr, &mVkSwapchainFramebuffers[i]));
		}
	}

	void VulkanModule::CreateTextureImage()
	{
		// Load image data from file

		Path texturePath = FileSystem::GetCurrentWorkingDirectory() / "Engine/Test/VulkanApp/Assets/Textures/Texture.jpg";

		int32 width, height, channels;
		stbi_uc* pixels = stbi_load(texturePath.GetString().c_str(), &width, &height, &channels, STBI_rgb_alpha);
		CS_ASSERT(pixels, "Failed to load texture");

		mTextureMipLevels = (uint32)std::floor(std::log2(std::max(width, height))) + 1;

		VkDeviceSize imageSize = width * height * 4;

		VkBuffer       stagingBuffer;
		VkDeviceMemory stagingBufferMemory;

		CreateBuffer(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

		void* data;
		VK_CALL(vkMapMemory(mVkDevice, stagingBufferMemory, 0, imageSize, 0, &data));
		memcpy(data, pixels, imageSize);
		vkUnmapMemory(mVkDevice, stagingBufferMemory);

		stbi_image_free(pixels);

		CreateImage({ (uint32)width, (uint32)height }, mTextureMipLevels, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, mVkTextureImage, mVkTextureImageMemory);
		TransitionImageLayout(mVkTextureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, mTextureMipLevels);
		CopyBufferToImage(stagingBuffer, mVkTextureImage, { (uint32)width, (uint32)height });
		GenerateMipmaps(mVkTextureImage, VK_FORMAT_R8G8B8A8_SRGB, { width, height }, mTextureMipLevels);

		vkDestroyBuffer(mVkDevice, stagingBuffer, nullptr);
		vkFreeMemory(mVkDevice, stagingBufferMemory, nullptr);
	}

	void VulkanModule::CreateTextureImageView()
	{
		mVkTextureImageView = CreateImageView(mVkTextureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT, mTextureMipLevels);
	}

	void VulkanModule::CreateTextureSampler()
	{
		VkPhysicalDeviceProperties properties = {};
		vkGetPhysicalDeviceProperties(mVkPhysicalDevice, &properties);

		VkSamplerCreateInfo createInfo     = {};
		createInfo.sType                   = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
		createInfo.minFilter               = VK_FILTER_LINEAR;
		createInfo.magFilter               = VK_FILTER_LINEAR;
		createInfo.addressModeU            = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		createInfo.addressModeV            = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		createInfo.addressModeW            = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		createInfo.anisotropyEnable        = VK_TRUE;
		createInfo.maxAnisotropy           = properties.limits.maxSamplerAnisotropy;
		createInfo.borderColor             = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
		createInfo.unnormalizedCoordinates = VK_FALSE;
		createInfo.mipmapMode              = VK_SAMPLER_MIPMAP_MODE_LINEAR;
		createInfo.mipLodBias              = 0.0f;
		createInfo.minLod                  = 0.0f;
		createInfo.maxLod                  = VK_LOD_CLAMP_NONE;

		VK_CALL(vkCreateSampler(mVkDevice, &createInfo, nullptr, &mVkTextureSampler));
	}

	void VulkanModule::CreateVertexBuffer()
	{
		VkBuffer       stagingBuffer;
		VkDeviceMemory stagingBufferMemory;

		CreateBuffer(sizeof(mVertices), VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

		void* data;
		VK_CALL(vkMapMemory(mVkDevice, stagingBufferMemory, 0, sizeof(mVertices), 0, &data));
		memcpy(data, mVertices, sizeof(mVertices));
		vkUnmapMemory(mVkDevice, stagingBufferMemory);

		CreateBuffer(sizeof(mVertices), VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, mVkVertexBuffer, mVkVertexBufferMemory);
		CopyBuffer(stagingBuffer, mVkVertexBuffer, sizeof(mVertices));

		vkDestroyBuffer(mVkDevice, stagingBuffer, nullptr);
		vkFreeMemory(mVkDevice, stagingBufferMemory, nullptr);
	}

	void VulkanModule::CreateIndexBuffer()
	{
		VkBuffer       stagingBuffer;
		VkDeviceMemory stagingBufferMemory;

		CreateBuffer(sizeof(mIndices), VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

		void* data;
		VK_CALL(vkMapMemory(mVkDevice, stagingBufferMemory, 0, sizeof(mIndices), 0, &data));
		memcpy(data, mIndices, sizeof(mIndices));
		vkUnmapMemory(mVkDevice, stagingBufferMemory);

		CreateBuffer(sizeof(mIndices), VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, mVkIndexBuffer, mVkIndexBufferMemory);
		CopyBuffer(stagingBuffer, mVkIndexBuffer, sizeof(mIndices));

		vkDestroyBuffer(mVkDevice, stagingBuffer, nullptr);
		vkFreeMemory(mVkDevice, stagingBufferMemory, nullptr);
	}

	void VulkanModule::CreateUniformBuffers()
	{
		mVkUniformBuffers.resize(sMaxFramesInFlight);
		mVkUniformBuffersMemory.resize(sMaxFramesInFlight);
		mUniformBuffersMapped.resize(sMaxFramesInFlight);

		for (size_t i = 0; i < sMaxFramesInFlight; i++)
		{
			CreateBuffer(sizeof(UniformBufferObject), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, mVkUniformBuffers[i], mVkUniformBuffersMemory[i]);

			vkMapMemory(mVkDevice, mVkUniformBuffersMemory[i], 0, sizeof(UniformBufferObject), 0, &mUniformBuffersMapped[i]);
		}
	}

	void VulkanModule::CreateDescriptorPool()
	{
		VkDescriptorPoolSize uboPoolSize = {};
		uboPoolSize.type                 = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		uboPoolSize.descriptorCount      = sMaxFramesInFlight;

		VkDescriptorPoolSize samplerPoolSize = {};
		samplerPoolSize.type                 = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		samplerPoolSize.descriptorCount      = sMaxFramesInFlight;

		VkDescriptorPoolSize poolSizes[] = { uboPoolSize, samplerPoolSize };

		VkDescriptorPoolCreateInfo createInfo = {};
		createInfo.sType                      = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		createInfo.poolSizeCount              = sizeof(poolSizes) / sizeof(poolSizes[0]);
		createInfo.pPoolSizes                 = poolSizes;
		createInfo.maxSets                    = sMaxFramesInFlight;
		createInfo.flags                      = 0;

		VK_CALL(vkCreateDescriptorPool(mVkDevice, &createInfo, nullptr, &mVkDescriptorPool));
	}

	void VulkanModule::CreateDescriptorSets()
	{
		Vector<VkDescriptorSetLayout> layouts(sMaxFramesInFlight, mVkDescriptorSetLayout);

		VkDescriptorSetAllocateInfo allocateInfo = {};
		allocateInfo.sType                       = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocateInfo.descriptorPool              = mVkDescriptorPool;
		allocateInfo.descriptorSetCount          = sMaxFramesInFlight;
		allocateInfo.pSetLayouts                 = layouts.data();

		mVkDescriptorSets.resize(sMaxFramesInFlight);
		VK_CALL(vkAllocateDescriptorSets(mVkDevice, &allocateInfo, mVkDescriptorSets.data()));

		for (size_t i = 0; i < sMaxFramesInFlight; i++)
		{
			VkDescriptorBufferInfo bufferInfo = {};
			bufferInfo.buffer                 = mVkUniformBuffers[i];
			bufferInfo.offset                 = 0;
			bufferInfo.range                  = sizeof(UniformBufferObject);

			VkDescriptorImageInfo imageInfo = {};
			imageInfo.imageLayout           = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			imageInfo.imageView             = mVkTextureImageView;
			imageInfo.sampler               = mVkTextureSampler;

			VkWriteDescriptorSet bufferDescriptorWrite = {};
			bufferDescriptorWrite.sType                = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			bufferDescriptorWrite.dstSet               = mVkDescriptorSets[i];
			bufferDescriptorWrite.dstBinding           = 0;
			bufferDescriptorWrite.dstArrayElement      = 0;
			bufferDescriptorWrite.descriptorType       = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			bufferDescriptorWrite.descriptorCount      = 1;
			bufferDescriptorWrite.pBufferInfo          = &bufferInfo;

			VkWriteDescriptorSet imageDescriptorWrite = {};
			imageDescriptorWrite.sType                = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			imageDescriptorWrite.dstSet               = mVkDescriptorSets[i];
			imageDescriptorWrite.dstBinding           = 1;
			imageDescriptorWrite.dstArrayElement      = 0;
			imageDescriptorWrite.descriptorType       = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			imageDescriptorWrite.descriptorCount      = 1;
			imageDescriptorWrite.pImageInfo           = &imageInfo;

			VkWriteDescriptorSet descriptorWrites[] = { bufferDescriptorWrite, imageDescriptorWrite };

			vkUpdateDescriptorSets(mVkDevice, sizeof(descriptorWrites) / sizeof(descriptorWrites[0]), descriptorWrites, 0, nullptr);
		}
	}

	void VulkanModule::CreateCommandBuffers()
	{
		mVkCommandBuffers.resize(sMaxFramesInFlight);

		VkCommandBufferAllocateInfo allocateInfo = {};
		allocateInfo.sType                       = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocateInfo.commandPool                 = mVkCommandPool;
		allocateInfo.level                       = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocateInfo.commandBufferCount          = mVkCommandBuffers.size();

		VK_CALL(vkAllocateCommandBuffers(mVkDevice, &allocateInfo, mVkCommandBuffers.data()));
	}

	void VulkanModule::CreateSynchronisationObjects()
	{
		mVkImageAvailableSemaphores.resize(sMaxFramesInFlight);
		mVkRenderFinishedSemaphores.resize(sMaxFramesInFlight);
		mVkInFlightFences.resize(sMaxFramesInFlight);

		VkSemaphoreCreateInfo semaphoreInfo = {};
		semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

		VkFenceCreateInfo fenceInfo = {};
		fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

		for (size_t i = 0; i < sMaxFramesInFlight; i++)
		{
			VK_CALL(vkCreateSemaphore(mVkDevice, &semaphoreInfo, nullptr, &mVkImageAvailableSemaphores[i]));
			VK_CALL(vkCreateSemaphore(mVkDevice, &semaphoreInfo, nullptr, &mVkRenderFinishedSemaphores[i]));
			VK_CALL(vkCreateFence(mVkDevice, &fenceInfo, nullptr, &mVkInFlightFences[i]));
		}
	}

	void VulkanModule::RecreateSwapchain()
	{
		vkDeviceWaitIdle(mVkDevice);

		CleanupSwapchain();

		CreateSwapchain();
		CreateImageViews();
		CreateDepthResources();
		CreateFramebuffers();
	}

	void VulkanModule::CleanupSwapchain()
	{
		vkDestroyImageView(mVkDevice, mVkDepthImageView, nullptr);
		vkDestroyImage(mVkDevice, mVkDepthImage, nullptr);
		vkFreeMemory(mVkDevice, mVkDepthImageMemory, nullptr);

		for (VkFramebuffer& framebuffer : mVkSwapchainFramebuffers)
			vkDestroyFramebuffer(mVkDevice, framebuffer, nullptr);

		for (VkImageView& imageView : mVkSwapchainImageViews)
			vkDestroyImageView(mVkDevice, imageView, nullptr);

		vkDestroySwapchainKHR(mVkDevice, mVkSwapchainKHR, nullptr);
	}

	bool VulkanModule::CheckValidationLayerSupport()
	{
		uint32 layerCount;
		VK_CALL(vkEnumerateInstanceLayerProperties(&layerCount, nullptr));

		VkLayerProperties* availableLayers = (VkLayerProperties*)alloca(layerCount * sizeof(VkLayerProperties));

		VK_CALL(vkEnumerateInstanceLayerProperties(&layerCount, availableLayers));

		bool valid = true;

		for (const char* requiredValidationLayer : mValidationLayers)
		{
			bool layerFound = false;

			for (uint32 i = 0; i < layerCount; i++)
			{
				const VkLayerProperties& layerProperties = availableLayers[i];

				if (strcmp(requiredValidationLayer, layerProperties.layerName) == 0)
				{
					layerFound = true;
					break;
				}
			}

			if (!layerFound)
			{
				CS_LOG_ERROR("Required Vulkan Validation Layer {} not found.", requiredValidationLayer);
				valid = false;
			}
		}

		return valid;
	}

	Vector<const char*> VulkanModule::GetRequiredExtensions()
	{
		uint32       glfwExtensionCount = 0;
		const char** glfwExtensions     = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

		Vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

		if (mEnableValidationLayers)
		{
			extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
		}

		return extensions;
	}

	void VulkanModule::PopulateVkDebugUtilsMessengerCreateInfoEXT(VkDebugUtilsMessengerCreateInfoEXT& createInfo)
	{
		VkDebugUtilsMessageSeverityFlagsEXT severityFlags = /*VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT | */
			VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;


		VkDebugUtilsMessageTypeFlagsEXT typeFlags = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_TYPE_DEVICE_ADDRESS_BINDING_BIT_EXT;

		createInfo.sType            = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		createInfo.messageSeverity  = severityFlags;
		createInfo.messageType      = typeFlags;
		createInfo.pfnUserCallback  = VulkanDebugCallback;
	}

	bool VulkanModule::IsDeviceSuitable(VkPhysicalDevice physicalDevice)
	{
		QueueFamilyIndices indices = FindQueueFamilies(physicalDevice);
		bool extensionsSupported = CheckDeviceExtensionSupport(physicalDevice);
		bool swapchainAdequate = false;

		if (extensionsSupported)
		{
			SwapchainSupportDetails supportDetails = QuerySwapchainSupport(physicalDevice);
			swapchainAdequate = supportDetails.IsAdequate();
		}

		VkPhysicalDeviceFeatures deviceFeatures;
		vkGetPhysicalDeviceFeatures(physicalDevice, &deviceFeatures);

		return indices.IsComplete() && extensionsSupported && swapchainAdequate && deviceFeatures.samplerAnisotropy;
	}

	bool VulkanModule::CheckDeviceExtensionSupport(VkPhysicalDevice physicalDevice)
	{
		uint32 extensionCount;
		vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, nullptr);

		VkExtensionProperties* availableExtensions = (VkExtensionProperties*)alloca(extensionCount * sizeof(VkExtensionProperties));
		vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, availableExtensions);

		bool valid = true;

		for (const char* requiredExtension : mDeviceExtensions)
		{
			bool found = false;

			for (uint32 i = 0; i < extensionCount; i++)
			{
				if (strcmp(requiredExtension, availableExtensions[i].extensionName) == 0)
				{
					found = true;
					break;
				}
			}

			if (!found)
				valid = false;
		}

		return valid;
	}

	QueueFamilyIndices VulkanModule::FindQueueFamilies(VkPhysicalDevice physicalDevice)
	{
		QueueFamilyIndices indices;

		uint32 queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);

		VkQueueFamilyProperties* queueFamilies = (VkQueueFamilyProperties*)alloca(queueFamilyCount * sizeof(VkQueueFamilyProperties));
		vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilies);

		for (uint32 i = 0; i < queueFamilyCount; i++)
		{
			const VkQueueFamilyProperties& queueFamily = queueFamilies[i];

			if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
			{
				indices.GraphicsFamily    = i;
				indices.HasGraphicsFamily = true;
			}

			VkBool32 presentSupport = VK_FALSE;
			vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, mVkSurfaceKHR, &presentSupport);

			if (presentSupport == VK_TRUE)
			{
				indices.PresentFamily    = i;
				indices.HasPresentFamily = true;
			}

			if (indices.IsComplete())
				break;
		}

		return indices;
	}

	SwapchainSupportDetails VulkanModule::QuerySwapchainSupport(VkPhysicalDevice physicalDevice)
	{
		SwapchainSupportDetails supportDetails;

		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, mVkSurfaceKHR, &supportDetails.Capabilities);

		uint32 formatCount;
		vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, mVkSurfaceKHR, &formatCount, nullptr);

		if (formatCount > 0)
		{
			supportDetails.Formats.resize(formatCount);
			vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, mVkSurfaceKHR, &formatCount, supportDetails.Formats.data());
		}

		uint32 presentModeCount;
		vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, mVkSurfaceKHR, &presentModeCount, nullptr);

		if (presentModeCount > 0)
		{
			supportDetails.PresentModes.resize(presentModeCount);
			vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, mVkSurfaceKHR, &presentModeCount, supportDetails.PresentModes.data());
		}

		return supportDetails;
	}

	VkSurfaceFormatKHR VulkanModule::ChooseSwapchainSurfaceFormat(const Vector<VkSurfaceFormatKHR>& availableFormats)
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

	VkPresentModeKHR VulkanModule::ChooseSwapchainPresentMode(const Vector<VkPresentModeKHR>& availablePresentModes)
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

	VkExtent2D VulkanModule::ChooseSwapchainExtent(const VkSurfaceCapabilitiesKHR& surfaceCapabilities)
	{
		if (surfaceCapabilities.currentExtent.width != std::numeric_limits<uint32>::max())
			return surfaceCapabilities.currentExtent;

		int2 framebufferSize;
		glfwGetFramebufferSize((GLFWwindow*)mWindow->GetHandle(), &framebufferSize.width, &framebufferSize.height);

		VkExtent2D actualExtent = {
			std::clamp((uint32)framebufferSize.width,  surfaceCapabilities.minImageExtent.width,  surfaceCapabilities.minImageExtent.width),
			std::clamp((uint32)framebufferSize.height, surfaceCapabilities.minImageExtent.height, surfaceCapabilities.minImageExtent.height)
		};

		return actualExtent;
	}

	VkImageView VulkanModule::CreateImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, uint32 mipLevels)
	{
		VkImageViewCreateInfo createInfo           = {};
		createInfo.sType                           = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		createInfo.image                           = image;
		createInfo.viewType                        = VK_IMAGE_VIEW_TYPE_2D;
		createInfo.format                          = format;
		createInfo.components.r                    = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.g                    = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.b                    = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.a                    = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.subresourceRange.aspectMask     = aspectFlags;
		createInfo.subresourceRange.baseMipLevel   = 0;
		createInfo.subresourceRange.levelCount     = mipLevels;
		createInfo.subresourceRange.baseArrayLayer = 0;
		createInfo.subresourceRange.layerCount     = 1;

		VkImageView imageView;
		VK_CALL(vkCreateImageView(mVkDevice, &createInfo, nullptr, &imageView));

		return imageView;
	}

	VkShaderModule VulkanModule::CreateShaderModule(const Buffer& bytecode)
	{
		VkShaderModuleCreateInfo createInfo = {};
		createInfo.sType                    = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		createInfo.codeSize                 = bytecode.GetSize();
		createInfo.pCode                    = reinterpret_cast<const uint32*>(bytecode.GetData());

		VkShaderModule shaderModule;
		VK_CALL(vkCreateShaderModule(mVkDevice, &createInfo, nullptr, &shaderModule));

		return shaderModule;
	}

	void VulkanModule::RecordCommandBuffer(VkCommandBuffer commandBuffer, uint32 imageIndex)
	{
		VkCommandBufferBeginInfo beginInfo = {};
		beginInfo.sType                    = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags                    = 0;
		beginInfo.pInheritanceInfo         = nullptr;

		VK_CALL(vkBeginCommandBuffer(commandBuffer, &beginInfo));

		VkViewport viewport = {};
		viewport.x          = 0.0f;
		viewport.y          = 0.0f;
		viewport.width      = (float32)mVkSwapchainExtent.width;
		viewport.height     = (float32)mVkSwapchainExtent.height;
		viewport.minDepth   = 0.0f;
		viewport.maxDepth   = 1.0f;

		VkRect2D scissorRect = {};
		scissorRect.offset   = { 0, 0 };
		scissorRect.extent   = mVkSwapchainExtent;

		VkClearValue clearValues[2];
		clearValues[0].color        = { { 0.0f, 0.0f, 0.0f, 1.0f } };
		clearValues[1].depthStencil = { 1.0f, 0 };

		VkRenderPassBeginInfo renderPassInfo = {};
		renderPassInfo.sType                 = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass            = mVkRenderPass;
		renderPassInfo.framebuffer           = mVkSwapchainFramebuffers[imageIndex];
		renderPassInfo.renderArea.offset     = { 0, 0 };
		renderPassInfo.renderArea.extent     = mVkSwapchainExtent;
		renderPassInfo.clearValueCount       = sizeof(clearValues) / sizeof(clearValues[0]);
		renderPassInfo.pClearValues          = clearValues;

		VkDeviceSize offsets[] = { 0 };

		vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

		vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, mVkGraphicsPipeline);
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, &mVkVertexBuffer, offsets);
		vkCmdBindIndexBuffer(commandBuffer, mVkIndexBuffer, 0, VK_INDEX_TYPE_UINT16);
		vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
		vkCmdSetScissor(commandBuffer, 0, 1, &scissorRect);
		vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, mVkPipelineLayout, 0, 1, &mVkDescriptorSets[imageIndex], 0, nullptr);
		vkCmdDrawIndexed(commandBuffer, sizeof(mIndices) / sizeof(mIndices[0]), 1, 0, 0, 0);

		vkCmdEndRenderPass(commandBuffer);

		VK_CALL(vkEndCommandBuffer(commandBuffer));
	}

	VkFormat VulkanModule::FindSupportedFormat(const Vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features)
	{
		for (VkFormat format : candidates)
		{
			VkFormatProperties properties;
			vkGetPhysicalDeviceFormatProperties(mVkPhysicalDevice, format, &properties);

			if (tiling == VK_IMAGE_TILING_LINEAR && (properties.linearTilingFeatures & features) == features)
				return format;
			else if (tiling == VK_IMAGE_TILING_OPTIMAL && (properties.optimalTilingFeatures & features) == features)
				return format;
		}

		CS_ASSERT(false, "Unsupported format");
		return VK_FORMAT_UNDEFINED;
	}

	VkFormat VulkanModule::FindDepthFormat()
	{
		return FindSupportedFormat({ VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT }, VK_IMAGE_TILING_OPTIMAL, VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
	}

	bool VulkanModule::HasStencilComponent(VkFormat format)
	{
		return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
	}

	void VulkanModule::CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory)
	{
		VkBufferCreateInfo bufferInfo = {};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = size;
		bufferInfo.usage = usage;
		bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		bufferInfo.flags = 0;

		VK_CALL(vkCreateBuffer(mVkDevice, &bufferInfo, nullptr, &buffer));

		VkMemoryRequirements memoryRequirements;
		vkGetBufferMemoryRequirements(mVkDevice, buffer, &memoryRequirements);

		VkMemoryAllocateInfo allocateInfo = {};
		allocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocateInfo.allocationSize = memoryRequirements.size;
		allocateInfo.memoryTypeIndex = FindMemoryType(memoryRequirements.memoryTypeBits, properties);

		VK_CALL(vkAllocateMemory(mVkDevice, &allocateInfo, nullptr, &bufferMemory));
		VK_CALL(vkBindBufferMemory(mVkDevice, buffer, bufferMemory, 0));
	}

	uint32 VulkanModule::FindMemoryType(uint32 typeFilter, VkMemoryPropertyFlags properties)
	{
		VkPhysicalDeviceMemoryProperties memoryProperties;
		vkGetPhysicalDeviceMemoryProperties(mVkPhysicalDevice, &memoryProperties);

		for (uint32 i = 0; i < memoryProperties.memoryTypeCount; i++)
		{
			if (typeFilter & (1 << i) && (memoryProperties.memoryTypes[i].propertyFlags & properties) == properties)
			{
				return i;
			}
		}

		return 0;
	}

	void VulkanModule::CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size)
	{
		VkCommandBuffer commandBuffer = BeginSingleTimeCommands();

		VkBufferCopy copyRegion = {};
		copyRegion.srcOffset    = 0;
		copyRegion.dstOffset    = 0;
		copyRegion.size         = size;

		vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

		EndSingleTimeCommands(commandBuffer);
	}

	void VulkanModule::UpdateUniformBuffer(uint32 currentImage)
	{
		float32 aspectRatio = mVkSwapchainExtent.width / (float32)mVkSwapchainExtent.height;

		glm::mat4 view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		glm::mat4 proj = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 10.0f);
		proj[1][1] *= -1;

		mTransformMatrix      = glm::rotate(glm::mat4(1.0f), Time::GetTime().InSeconds(), glm::vec3(0.0f, 0.0f, 1.0f));
		mViewProjectionMatrix = proj * view;

		UniformBufferObject ubo;
		ubo.ViewProjectionMatrix = mViewProjectionMatrix;
		ubo.TransformMatrix      = mTransformMatrix;

		memcpy(mUniformBuffersMapped[currentImage], &ubo, sizeof(UniformBufferObject));
	}

	void VulkanModule::CreateImage(uint2 size, uint32 mipLevels, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory)
	{
		VkImageCreateInfo createInfo = {};
		createInfo.sType             = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		createInfo.imageType         = VK_IMAGE_TYPE_2D;
		createInfo.extent.width      = size.width;
		createInfo.extent.height     = size.height;
		createInfo.extent.depth      = 1;
		createInfo.mipLevels         = mipLevels;
		createInfo.arrayLayers       = 1;
		createInfo.format            = format;
		createInfo.tiling            = tiling;
		createInfo.initialLayout     = VK_IMAGE_LAYOUT_UNDEFINED;
		createInfo.usage             = usage;
		createInfo.sharingMode       = VK_SHARING_MODE_EXCLUSIVE;
		createInfo.samples           = VK_SAMPLE_COUNT_1_BIT;
		createInfo.flags             = 0;

		VK_CALL(vkCreateImage(mVkDevice, &createInfo, nullptr, &image));

		VkMemoryRequirements memoryRequirements;
		vkGetImageMemoryRequirements(mVkDevice, image, &memoryRequirements);

		VkMemoryAllocateInfo allocateInfo = {};
		allocateInfo.sType                = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocateInfo.allocationSize       = memoryRequirements.size;
		allocateInfo.memoryTypeIndex      = FindMemoryType(memoryRequirements.memoryTypeBits, properties);

		VK_CALL(vkAllocateMemory(mVkDevice, &allocateInfo, nullptr, &imageMemory));
		VK_CALL(vkBindImageMemory(mVkDevice, image, imageMemory, 0));
	}

	VkCommandBuffer VulkanModule::BeginSingleTimeCommands()
	{
		VkCommandBufferAllocateInfo allocateInfo = {};
		allocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocateInfo.commandBufferCount = 1;
		allocateInfo.commandPool = mVkCommandPool;
		allocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;

		VkCommandBuffer commandBuffer;
		VK_CALL(vkAllocateCommandBuffers(mVkDevice, &allocateInfo, &commandBuffer));

		VkCommandBufferBeginInfo beginInfo = {};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

		VK_CALL(vkBeginCommandBuffer(commandBuffer, &beginInfo));

		return commandBuffer;
	}

	void VulkanModule::EndSingleTimeCommands(VkCommandBuffer commandBuffer)
	{
		VK_CALL(vkEndCommandBuffer(commandBuffer));

		VkSubmitInfo submitInfo = {};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &commandBuffer;

		VK_CALL(vkQueueSubmit(mVkGraphicsQueue, 1, &submitInfo, VK_NULL_HANDLE));
		VK_CALL(vkQueueWaitIdle(mVkGraphicsQueue));

		vkFreeCommandBuffers(mVkDevice, mVkCommandPool, 1, &commandBuffer);
	}

	void VulkanModule::TransitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, uint32 mipLevels)
	{
		VkCommandBuffer commandBuffer = BeginSingleTimeCommands();

		VkImageMemoryBarrier barrier            = {};
		barrier.sType                           = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		barrier.oldLayout                       = oldLayout;
		barrier.newLayout                       = newLayout;
		barrier.srcQueueFamilyIndex             = VK_QUEUE_FAMILY_IGNORED;
		barrier.dstQueueFamilyIndex             = VK_QUEUE_FAMILY_IGNORED;
		barrier.image                           = image;
		barrier.subresourceRange.baseMipLevel   = 0;
		barrier.subresourceRange.levelCount     = mipLevels;
		barrier.subresourceRange.baseArrayLayer = 0;
		barrier.subresourceRange.layerCount     = 1;

		VkPipelineStageFlags srcStage, dstStage;

		if (newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL)
		{
			barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;

			if (HasStencilComponent(format))
				barrier.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
		}
		else
		{
			barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		}

		if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
		{
			barrier.srcAccessMask = 0;
			barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

			srcStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
			dstStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
		}
		else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
		{
			barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
			barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

			srcStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
			dstStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
		}
		else if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL)
		{
			barrier.srcAccessMask = 0;
			barrier.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

			srcStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
			dstStage = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
		}
		else
		{
			CS_ASSERT(false, "Unsupported image layout transition.");
		}

		vkCmdPipelineBarrier(commandBuffer, srcStage, dstStage, 0, 0, nullptr, 0, nullptr, 1, &barrier);

		EndSingleTimeCommands(commandBuffer);
	}

	void VulkanModule::CopyBufferToImage(VkBuffer buffer, VkImage image, uint2 size)
	{
		VkCommandBuffer commandBuffer = BeginSingleTimeCommands();

		VkBufferImageCopy region = {};
		region.bufferOffset = 0;
		region.bufferRowLength = 0;
		region.bufferImageHeight = 0;
		region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		region.imageSubresource.mipLevel = 0;
		region.imageSubresource.baseArrayLayer = 0;
		region.imageSubresource.layerCount = 1;
		region.imageOffset = { 0, 0, 0 };
		region.imageExtent = { size.width, size.height, 1 };

		vkCmdCopyBufferToImage(commandBuffer, buffer, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

		EndSingleTimeCommands(commandBuffer);
	}

	void VulkanModule::GenerateMipmaps(VkImage image, VkFormat imageFormat, int2 size, uint32 mipLevels)
	{
		VkFormatProperties formatProperties;
		vkGetPhysicalDeviceFormatProperties(mVkPhysicalDevice, imageFormat, &formatProperties);

		CS_ASSERT(formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT, "Texture image format doesn't support linear blitting");

		VkCommandBuffer commandBuffer = BeginSingleTimeCommands();

		VkImageMemoryBarrier barrier            = {};
		barrier.sType                           = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		barrier.image                           = image;
		barrier.srcQueueFamilyIndex             = VK_QUEUE_FAMILY_IGNORED;
		barrier.dstQueueFamilyIndex             = VK_QUEUE_FAMILY_IGNORED;
		barrier.subresourceRange.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT;
		barrier.subresourceRange.baseArrayLayer = 0;
		barrier.subresourceRange.layerCount     = 1;
		barrier.subresourceRange.levelCount     = 1;

		int32 mipWidth  = size.width;
		int32 mipHeight = size.height;

		for (uint32 i = 1; i < mipLevels; i++)
		{
			barrier.subresourceRange.baseMipLevel = i - 1;
			barrier.oldLayout                     = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
			barrier.newLayout                     = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
			barrier.srcAccessMask                 = VK_ACCESS_TRANSFER_WRITE_BIT;
			barrier.dstAccessMask                 = VK_ACCESS_TRANSFER_READ_BIT;

			vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, nullptr, 0, nullptr, 1, &barrier);

			VkImageBlit blit                   = {};
			blit.srcOffsets[0]                 = { 0, 0, 0 };
			blit.srcOffsets[1]                 = { mipWidth, mipHeight, 1 };
			blit.srcSubresource.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT;
			blit.srcSubresource.mipLevel       = i - 1;
			blit.srcSubresource.baseArrayLayer = 0;
			blit.srcSubresource.layerCount     = 1;
			blit.dstOffsets[0]                 = { 0, 0, 0 };
			blit.dstOffsets[1]                 = { mipWidth > 1 ? mipWidth / 2 : 1, mipHeight > 1 ? mipHeight / 2 : 1, 1 };
			blit.dstSubresource.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT;
			blit.dstSubresource.mipLevel       = i;
			blit.dstSubresource.baseArrayLayer = 0;
			blit.dstSubresource.layerCount     = 1;

			vkCmdBlitImage(commandBuffer, image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &blit, VK_FILTER_LINEAR);

			barrier.oldLayout     = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
			barrier.newLayout     = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
			barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

			vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, nullptr, 0, nullptr, 1, &barrier);

			mipWidth  = mipWidth  > 1 ? mipWidth  / 2 : 1;
			mipHeight = mipHeight > 1 ? mipHeight / 2 : 1;
		}

		barrier.subresourceRange.baseMipLevel = mipLevels - 1;
		barrier.oldLayout                     = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
		barrier.newLayout                     = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		barrier.srcAccessMask                 = VK_ACCESS_TRANSFER_READ_BIT;
		barrier.dstAccessMask                 = VK_ACCESS_SHADER_READ_BIT;

		vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, nullptr, 0, nullptr, 1, &barrier);

		EndSingleTimeCommands(commandBuffer);
	}

}