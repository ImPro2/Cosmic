#include "cspch.hpp"
#include "VulkanModule.hpp"
#include "VulkanDebugUtils.hpp"

#include "App/Application.hpp"

#include <fstream>

CS_MODULE_LOG_INFO(VulkanApp, VulkanModule);

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
		mWindow = CreateScope<IVulkanDesktopWindow>(DesktopWindowInfo());
		mWindow->SetCloseCallback([this]() { Application::Get()->Close(); });

		CreateInstance();
		SetupDebugMessenger();
		CreateSurface();
		PickPhysicalDevice();
		CreateLogicalDevice();
		CreateSwapchain();
		CreateImageViews();
		CreateRenderPass();
		CreateGraphicsPipeline();
		CreateFramebuffers();
		CreateCommandPool();
		CreateCommandBuffer();
		CreateSynchronisationObjects();
	}

	void VulkanModule::OnShutdown()
	{
		vkDeviceWaitIdle(mVkDevice);

		vkDestroySemaphore(mVkDevice, mVkImageAvailableSemaphore, nullptr);
		vkDestroySemaphore(mVkDevice, mVkRenderFinishedSemaphore, nullptr);
		vkDestroyFence(mVkDevice, mVkInFlightFence, nullptr);

		vkFreeCommandBuffers(mVkDevice, mVkCommandPool, 1, &mVkCommandBuffer);
		vkDestroyCommandPool(mVkDevice, mVkCommandPool, nullptr);

		for (VkFramebuffer& framebuffer : mVkSwapchainFramebuffers)
			vkDestroyFramebuffer(mVkDevice, framebuffer, nullptr);

		vkDestroyPipeline(mVkDevice, mVkGraphicsPipeline, nullptr);
		vkDestroyPipelineLayout(mVkDevice, mVkPipelineLayout, nullptr);
		vkDestroyRenderPass(mVkDevice, mVkRenderPass, nullptr);

		for (VkImageView& imageView : mVkSwapchainImageViews)
			vkDestroyImageView(mVkDevice, imageView, nullptr);

		vkDestroySwapchainKHR(mVkDevice, mVkSwapchainKHR, nullptr);
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

		VK_CALL(vkWaitForFences(mVkDevice, 1, &mVkInFlightFence, VK_TRUE, std::numeric_limits<uint64>::max()));
		VK_CALL(vkResetFences(mVkDevice, 1, &mVkInFlightFence));

		uint32 imageIndex;
		VK_CALL(vkAcquireNextImageKHR(mVkDevice, mVkSwapchainKHR, std::numeric_limits<uint64>::max(), mVkImageAvailableSemaphore, VK_NULL_HANDLE, &imageIndex));

		VK_CALL(vkResetCommandBuffer(mVkCommandBuffer, 0));

		RecordCommandBuffer(mVkCommandBuffer, imageIndex);

		VkSemaphore          waitSemaphores[]   = { mVkImageAvailableSemaphore                    };
		VkSemaphore          signalSemaphores[] = { mVkRenderFinishedSemaphore                    };
		VkPipelineStageFlags waitStages[]       = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };

		VkSubmitInfo submitInfo         = {};
		submitInfo.sType                = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.waitSemaphoreCount   = 1;
		submitInfo.pWaitSemaphores      = waitSemaphores;
		submitInfo.pWaitDstStageMask    = waitStages;
		submitInfo.commandBufferCount   = 1;
		submitInfo.pCommandBuffers      = &mVkCommandBuffer;
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores    = signalSemaphores;

		VK_CALL(vkQueueSubmit(mVkGraphicsQueue, 1, &submitInfo, mVkInFlightFence));

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

		static TimeUnit lastTime = Time::GetTime();
		TimeUnit currTime = Time::GetTime();

		if (currTime.InSeconds() - lastTime.InSeconds() > 5.0f)
		{
			lastTime = currTime;
			CS_LOG_DEBUG("DeltaTime: {}ms", Time::GetFPS().InSeconds());
		}
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
			VkImageViewCreateInfo createInfo           = {};
			createInfo.sType                           = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			createInfo.image                           = mVkSwapchainImages[i];
			createInfo.viewType                        = VK_IMAGE_VIEW_TYPE_2D;
			createInfo.format                          = mVkSwapchainImageFormat;
			createInfo.components.r                    = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.g                    = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.b                    = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.a                    = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.subresourceRange.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT;
			createInfo.subresourceRange.baseMipLevel   = 0;
			createInfo.subresourceRange.levelCount     = 1;
			createInfo.subresourceRange.baseArrayLayer = 0;
			createInfo.subresourceRange.layerCount     = 1;

			VK_CALL(vkCreateImageView(mVkDevice, &createInfo, nullptr, &mVkSwapchainImageViews[i]));
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

		VkAttachmentReference colorAttachmentRef = {};
		colorAttachmentRef.attachment            = 0;
		colorAttachmentRef.layout                = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		VkSubpassDescription subpass = {};
		subpass.pipelineBindPoint    = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpass.colorAttachmentCount = 1;
		subpass.pColorAttachments    = &colorAttachmentRef;

		VkSubpassDependency dependency = {};
		dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
		dependency.dstSubpass = 0;
		dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependency.srcAccessMask = 0;
		dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

		VkRenderPassCreateInfo createInfo = {};
		createInfo.sType                  = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		createInfo.attachmentCount        = 1;
		createInfo.pAttachments           = &colorAttachment;
		createInfo.subpassCount           = 1;
		createInfo.pSubpasses             = &subpass;
		createInfo.dependencyCount        = 1;
		createInfo.pDependencies          = &dependency;

		VK_CALL(vkCreateRenderPass(mVkDevice, &createInfo, nullptr, &mVkRenderPass));
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

		VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
		vertexInputInfo.sType                                = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		vertexInputInfo.vertexBindingDescriptionCount        = 0;
		vertexInputInfo.pVertexBindingDescriptions           = nullptr;
		vertexInputInfo.vertexAttributeDescriptionCount      = 0;
		vertexInputInfo.pVertexAttributeDescriptions         = nullptr;

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
		rasterizationInfo.frontFace                              = VK_FRONT_FACE_CLOCKWISE;
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

		VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
		pipelineLayoutInfo.sType                      = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount             = 0;
		pipelineLayoutInfo.pSetLayouts                = nullptr;
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
		createInfo.pDepthStencilState           = nullptr;
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

	void VulkanModule::CreateFramebuffers()
	{
		mVkSwapchainFramebuffers.resize(mVkSwapchainImageViews.size());

		for (size_t i = 0; i < mVkSwapchainImageViews.size(); i++)
		{
			VkImageView attachments[] = { mVkSwapchainImageViews[i] };

			VkFramebufferCreateInfo createInfo = {};
			createInfo.sType                   = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			createInfo.renderPass              = mVkRenderPass;
			createInfo.attachmentCount         = 1;
			createInfo.pAttachments            = attachments;
			createInfo.width                   = mVkSwapchainExtent.width;
			createInfo.height                  = mVkSwapchainExtent.height;
			createInfo.layers                  = 1;

			VK_CALL(vkCreateFramebuffer(mVkDevice, &createInfo, nullptr, &mVkSwapchainFramebuffers[i]));
		}
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

	void VulkanModule::CreateCommandBuffer()
	{
		VkCommandBufferAllocateInfo allocateInfo = {};
		allocateInfo.sType                       = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocateInfo.commandPool                 = mVkCommandPool;
		allocateInfo.level                       = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocateInfo.commandBufferCount          = 1;

		VK_CALL(vkAllocateCommandBuffers(mVkDevice, &allocateInfo, &mVkCommandBuffer));
	}

	void VulkanModule::CreateSynchronisationObjects()
	{
		VkSemaphoreCreateInfo semaphoreInfo = {};
		semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

		VkFenceCreateInfo fenceInfo = {};
		fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

		VK_CALL(vkCreateSemaphore(mVkDevice, &semaphoreInfo, nullptr, &mVkImageAvailableSemaphore));
		VK_CALL(vkCreateSemaphore(mVkDevice, &semaphoreInfo, nullptr, &mVkRenderFinishedSemaphore));
		VK_CALL(vkCreateFence(mVkDevice, &fenceInfo, nullptr, &mVkInFlightFence));
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
		QueueFamilyIndices indices             = FindQueueFamilies(physicalDevice);
		bool               extensionsSupported = CheckDeviceExtensionSupport(physicalDevice);
		bool               swapchainAdequate   = false;

		if (extensionsSupported)
		{
			SwapchainSupportDetails supportDetails = QuerySwapchainSupport(physicalDevice);
			swapchainAdequate = supportDetails.IsAdequate();
		}

		return indices.IsComplete() && extensionsSupported && swapchainAdequate;
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

		VK_CALL(vkBeginCommandBuffer(mVkCommandBuffer, &beginInfo));

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

		VkClearValue clearColor = VkClearValue { VkClearColorValue { { 0.0f, 0.0f, 0.0f, 1.0f } } };

		VkRenderPassBeginInfo renderPassInfo = {};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = mVkRenderPass;
		renderPassInfo.framebuffer = mVkSwapchainFramebuffers[imageIndex];
		renderPassInfo.renderArea.offset = { 0, 0 };
		renderPassInfo.renderArea.extent = mVkSwapchainExtent;
		renderPassInfo.clearValueCount = 1;
		renderPassInfo.pClearValues = &clearColor;

		vkCmdBeginRenderPass(mVkCommandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

		vkCmdBindPipeline(mVkCommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, mVkGraphicsPipeline);
		vkCmdSetViewport(mVkCommandBuffer, 0, 1, &viewport);
		vkCmdSetScissor(mVkCommandBuffer, 0, 1, &scissorRect);
		vkCmdDraw(mVkCommandBuffer, 3, 1, 0, 0);

		vkCmdEndRenderPass(mVkCommandBuffer);

		VK_CALL(vkEndCommandBuffer(mVkCommandBuffer));
	}

}