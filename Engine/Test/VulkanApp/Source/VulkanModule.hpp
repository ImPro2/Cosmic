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

#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#undef CreateWindow
#undef CreateWindowW

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

	struct Vertex
	{
		float3 Position;
		float3 Color;
		float2 TexCoord;

		static VkVertexInputBindingDescription GetVkVertexInputBindingDescription()
		{
			VkVertexInputBindingDescription bindingDescription = {};
			bindingDescription.binding                         = 0;
			bindingDescription.stride                          = sizeof(Vertex);
			bindingDescription.inputRate                       = VK_VERTEX_INPUT_RATE_VERTEX;

			return bindingDescription;
		}

		static Vector<VkVertexInputAttributeDescription> GetVkVertexInputAttributeDescriptions()
		{
			Vector<VkVertexInputAttributeDescription> attributeDescriptions(3);

			attributeDescriptions[0].binding  = 0;
			attributeDescriptions[0].location = 0;
			attributeDescriptions[0].format   = VK_FORMAT_R32G32B32_SFLOAT;
			attributeDescriptions[0].offset   = offsetof(Vertex, Position);

			attributeDescriptions[1].binding  = 0;
			attributeDescriptions[1].location = 1;
			attributeDescriptions[1].format   = VK_FORMAT_R32G32B32_SFLOAT;
			attributeDescriptions[1].offset   = offsetof(Vertex, Color);

			attributeDescriptions[2].binding  = 0;
			attributeDescriptions[2].location = 2;
			attributeDescriptions[2].format   = VK_FORMAT_R32G32_SFLOAT;
			attributeDescriptions[2].offset   = offsetof(Vertex, TexCoord);

			return attributeDescriptions;
		}
	};

	struct UniformBufferObject
	{
		glm::mat4 ViewProjectionMatrix;
		glm::mat4 TransformMatrix;
	};

	class VulkanModule : public IModule
	{
	public:
		void OnInit()        override;
		void OnShutdown()    override;
		void OnUpdate(Dt dt) override;
		void OnEvent(const IEvent& e) override;

	private:
		bool OnWindowResize(const WindowResizeEvent& e);

	private:
		void CreateWindow();
		void CreateInstance();
		void SetupDebugMessenger();
		void CreateSurface();
		void PickPhysicalDevice();
		void CreateLogicalDevice();
		void CreateSwapchain();
		void CreateImageViews();
		void CreateRenderPass();
		void CreateDescriptorSetLayout();
		void CreateGraphicsPipeline();
		void CreateCommandPool();
		void CreateDepthResources();
		void CreateFramebuffers();
		void CreateTextureImage();
		void CreateTextureImageView();
		void CreateTextureSampler();
		void CreateVertexBuffer();
		void CreateIndexBuffer();
		void CreateUniformBuffers();
		void CreateDescriptorPool();
		void CreateDescriptorSets();
		void CreateCommandBuffers();
		void CreateSynchronisationObjects();

		void RecreateSwapchain();
		void CleanupSwapchain();

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
		VkImageView CreateImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, uint32 mipLevels);
		VkShaderModule CreateShaderModule(const Buffer& bytecode);
		void RecordCommandBuffer(VkCommandBuffer commandBuffer, uint32 imageIndex);
		VkFormat FindSupportedFormat(const Vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
		VkFormat FindDepthFormat();
		bool HasStencilComponent(VkFormat format);
		void CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
		uint32 FindMemoryType(uint32 typeFilter, VkMemoryPropertyFlags properties);
		void CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
		void UpdateUniformBuffer(uint32 currentImage);
		void CreateImage(uint2 size, uint32 mipLevels, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
		VkCommandBuffer BeginSingleTimeCommands();
		void EndSingleTimeCommands(VkCommandBuffer commandBuffer);
		void TransitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, uint32 mipLevels);
		void CopyBufferToImage(VkBuffer buffer, VkImage image, uint2 size);
		void GenerateMipmaps(VkImage image, VkFormat imageFormat, int2 size, uint32 mipLevels);

	private:
		Scope<IVulkanDesktopWindow> mWindow;

		inline static const uint32 sMaxFramesInFlight = 3;

		uint32 mCurrentFrameIndex  = 0;
		bool   mFramebufferResized = false;

		Vertex mVertices[8] = {
			Vertex { float3 { -0.5f, -0.5f, 0.0f }, float3 { 1.0f, 0.0f, 0.0f }, float2 { 1.0f, 0.0f } },
			Vertex { float3 {  0.5f, -0.5f, 0.0f }, float3 { 0.0f, 1.0f, 0.0f }, float2 { 0.0f, 0.0f } },
			Vertex { float3 {  0.5f,  0.5f, 0.0f }, float3 { 0.0f, 0.0f, 1.0f }, float2 { 0.0f, 1.0f } },
			Vertex { float3 { -0.5f,  0.5f, 0.0f }, float3 { 1.0f, 1.0f, 1.0f }, float2 { 1.0f, 1.0f } },

			Vertex { float3 { -0.5f, -0.5f, -0.5f }, float3 { 1.0f, 0.0f, 0.0f }, float2 { 1.0f, 0.0f } },
			Vertex { float3 {  0.5f, -0.5f, -0.5f }, float3 { 0.0f, 1.0f, 0.0f }, float2 { 0.0f, 0.0f } },
			Vertex { float3 {  0.5f,  0.5f, -0.5f }, float3 { 0.0f, 0.0f, 1.0f }, float2 { 0.0f, 1.0f } },
			Vertex { float3 { -0.5f,  0.5f, -0.5f }, float3 { 1.0f, 1.0f, 1.0f }, float2 { 1.0f, 1.0f } }
		};

		uint16 mIndices[12] = {
			0, 1, 2, 2, 3, 0,
			4, 5, 6, 6, 7, 4
		};

		glm::mat4 mViewProjectionMatrix;
		glm::mat4 mTransformMatrix;

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
		VkDescriptorSetLayout    mVkDescriptorSetLayout;
		VkPipelineLayout         mVkPipelineLayout;
		VkPipeline               mVkGraphicsPipeline;
		Vector<VkFramebuffer>    mVkSwapchainFramebuffers;
		VkCommandPool            mVkCommandPool;
		Vector<VkCommandBuffer>  mVkCommandBuffers;
		Vector<VkSemaphore>      mVkImageAvailableSemaphores;
		Vector<VkSemaphore>      mVkRenderFinishedSemaphores;
		Vector<VkFence>          mVkInFlightFences;
		VkBuffer                 mVkVertexBuffer;
		VkDeviceMemory           mVkVertexBufferMemory;
		VkBuffer                 mVkIndexBuffer;
		VkDeviceMemory           mVkIndexBufferMemory;
		Vector<VkBuffer>         mVkUniformBuffers;
		Vector<VkDeviceMemory>   mVkUniformBuffersMemory;
		Vector<void*>            mUniformBuffersMapped;
		VkDescriptorPool         mVkDescriptorPool;
		Vector<VkDescriptorSet>  mVkDescriptorSets;
		uint32                   mTextureMipLevels;
		VkImage                  mVkTextureImage;
		VkDeviceMemory           mVkTextureImageMemory;
		VkImageView              mVkTextureImageView;
		VkSampler                mVkTextureSampler;
		VkImage                  mVkDepthImage;
		VkDeviceMemory           mVkDepthImageMemory;
		VkImageView              mVkDepthImageView;
	};

}
