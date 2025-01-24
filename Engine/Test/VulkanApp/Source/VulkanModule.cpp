#include "cspch.hpp"
#include "VulkanModule.hpp"
#include "VulkanDebugUtils.hpp"

#include "App/Application.hpp"

#include <stb_image.h>
#include <glm/gtc/matrix_transform.hpp>

#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

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
		DesktopWindowInfo info;
		info.Title = "Vulkan App";

		mWindow = CreateScope<IVulkanDesktopWindow>(info);

		const Ref<VulkanGraphicsContext>& graphicsContext = mWindow.As<IVulkanDesktopWindow>()->GetVulkanGraphicsContext();

		mInstance = graphicsContext->GetInstance();
		mPhysicalDevice = graphicsContext->GetPhysicalDevice();
		mDevice = graphicsContext->GetDevice();
		mSwapchain = graphicsContext->GetSwapchain();
		mGraphicsQueue = graphicsContext->GetGraphicsQueue();
		mPresentQueue = graphicsContext->GetPresentQueue();

		ShaderInfo shaderInfo = {
			.Stages         = EShaderStage::All,
			.SourceLanguage = EShaderSourceLanguage::GLSL,
			.Name           = "Shader",
			.FilePath       = FileSystem::GetCurrentWorkingDirectory() / "Engine/Test/VulkanApp/Assets/Shaders/Shader.glsl"
		};

		mShader = CreateRef<VulkanShader>(shaderInfo, mDevice);

		AttachmentInfo colorAttachment = {
			.Size    = { 0, 0},
			.Samples = 1,
			.Format  = ETextureFormat::Unknown,
			.Flags   = EAttachmentFlags::SwapchainTarget,
			.Type    = EAttachmentType::ColorAttachment,
			.Name    = "Color Attachment"
		};

		AttachmentInfo depthAttachment = {
			.Size    = { 0, 0},
			.Samples = 1,
			.Format  = ETextureFormat::Depth16_UNorm_Stencil8_UInt,
			.Flags   = EAttachmentFlags::None,
			.Type    = EAttachmentType::DepthAttachment,
			.Name    = "Depth Attachment",
		};

		mRenderPass = CreateRef<VulkanRenderPass>("asdf", mSwapchain, mDevice);
		mRenderPass->AddColorOutput(colorAttachment);
		mRenderPass->SetDepthStencilOutput(depthAttachment);
		mRenderPass->CreateHandle();

		GraphicsPipelineInfo pipelineInfo = {
			.InputLayout        = mShader->GetReflectionData().InputLayout,
			.PrimitiveTopology  = EPrimitiveTopology::TriangleList,
			.CullMode           = ECullMode::Back,
			.EnableDepthTesting = true,
			.Shader             = mShader,
		};

		mPipeline = CreateRef<VulkanGraphicsPipeline>(pipelineInfo, mRenderPass, mDevice);
		mPipeline->CreateHandle();

		const ShaderReflectionData& reflectionData = mShader->GetReflectionData();
		UnorderedMap<EShaderResourceType, uint32> descriptorTypeCountMap;

		for (const ShaderDescriptorSetMetadata& descriptorSetMetadata : reflectionData.DescriptorSets)
		{
			for (const ShaderResourceMetadata& shaderResourceMetadata : descriptorSetMetadata.Bindings)
			{
				descriptorTypeCountMap[shaderResourceMetadata.Type]++;
			}
		}

		mFramesInFlight.resize(sMaxFramesInFlight);

		Vector<VkDescriptorSetLayout> descriptorSetLayouts(mFramesInFlight.size(), mShader->GetDescriptorSetLayouts()[0]);

		// Create 3 descriptor sets, one for each frame in flight
		mDescriptorPool = CreateRef<VulkanDescriptorPool>(mDevice, mFramesInFlight.size(), descriptorTypeCountMap);
		Vector<Ref<VulkanDescriptorSet>> descriptorSets = mDescriptorPool->AllocateDescriptorSets(descriptorSetLayouts);

		mCommandPool = CreateRef<VulkanCommandPool>(mDevice, mGraphicsQueue->GetQueueFamilyIndex(), VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);
		Vector<Ref<VulkanCommandBuffer>> commandBuffers = mCommandPool->AllocateCommandBuffers(mFramesInFlight.size(), VK_COMMAND_BUFFER_LEVEL_PRIMARY);

		// Vertex & index buffers

		{
			Ref<VulkanBuffer> stagingBuffer = CreateRef<VulkanBuffer>(mDevice, sizeof(mVertices), VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
			stagingBuffer->GetDeviceMemory()->CopyData(0, sizeof(mVertices), mVertices);

			mVertexBuffer = CreateRef<VulkanBuffer>(mDevice, sizeof(mVertices), VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

			mGraphicsQueue->SubmitSingleTimeCommands(mCommandPool, nullptr, [&](const Ref<VulkanCommandBuffer>& commandBuffer)
			{
				commandBuffer->CopyBuffer(stagingBuffer, mVertexBuffer);
			});
		}

		{
			Ref<VulkanBuffer> stagingBuffer = CreateRef<VulkanBuffer>(mDevice, sizeof(mIndices), VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
			stagingBuffer->GetDeviceMemory()->CopyData(0, sizeof(mIndices), mIndices);

			mIndexBuffer = CreateRef<VulkanBuffer>(mDevice, sizeof(mIndices), VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

			mGraphicsQueue->SubmitSingleTimeCommands(mCommandPool, nullptr, [&](const Ref<VulkanCommandBuffer>& commandBuffer)
			{
				commandBuffer->CopyBuffer(stagingBuffer, mIndexBuffer);
			});
		}

		// Texture image

		{
			Path texturePath = FileSystem::GetCurrentWorkingDirectory() / "Engine/Test/VulkanApp/Assets/Textures/Texture.jpg";

			int32 width, height, channels;
			stbi_uc* pixels = stbi_load(texturePath.GetString().c_str(), &width, &height, &channels, STBI_rgb_alpha);
			CS_ASSERT(pixels, "Failed to load texture");

			uint32 textureMipLevels = (uint32)std::floor(std::log2(std::max(width, height))) + 1;

			VkDeviceSize imageSize = width * height * 4;

			mTextureImage = CreateRef<VulkanImage>(
				mDevice,
				uint2 { (uint32)width, (uint32)height },
//				textureMipLevels,
				1,
				VK_FORMAT_R8G8B8A8_SRGB,
				VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
				VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
				VK_IMAGE_ASPECT_COLOR_BIT,
				true
			);

			Ref<VulkanBuffer> stagingBuffer = CreateRef<VulkanBuffer>(mDevice, imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
			stagingBuffer->GetDeviceMemory()->CopyData(0, imageSize, pixels);

			mGraphicsQueue->SubmitSingleTimeCommands(mCommandPool, nullptr, [&](const Ref<VulkanCommandBuffer>& commandBuffer)
			{
				// Transition image layout
				// Copy buffer to image
				// Generate mipmaps

				commandBuffer->TransitionImageLayout(mTextureImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
				commandBuffer->CopyBufferToImage(stagingBuffer, mTextureImage);
				commandBuffer->TransitionImageLayout(mTextureImage, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

				// TODO: Create mipmaps
			});
		}

		// Depth resources

		mDepthTextureImage = CreateRef<VulkanImage>(
			mDevice,
			uint2 { mSwapchain->GetExtent().width, mSwapchain->GetExtent().height },
			1,
			VK_FORMAT_D16_UNORM_S8_UINT,
			VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
			VK_IMAGE_ASPECT_DEPTH_BIT,
			false
		);

		// Framebuffers

		const Vector<VkImageView>& swapchainImageViews = mSwapchain->GetImageViewHandles();

		mFramebuffers.resize(swapchainImageViews.size());
		for (uint32 i = 0; i < swapchainImageViews.size(); i++)
		{
			mFramebuffers[i] = CreateRef<VulkanFramebuffer>(mDevice, mRenderPass, Vector<VkImageView>({ swapchainImageViews[i], mDepthTextureImage->GetImageViewHandle() }));
		}

		// Frame resources

		for (uint32 i = 0; i < mFramesInFlight.size(); i++)
		{
			FrameData& frameData = mFramesInFlight[i];

			frameData.AcquireNextImageFence   = CreateRef<VulkanFence>(mDevice, true);
			frameData.ImageAvailableSemaphore = CreateRef<VulkanSemaphore>(mDevice, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT);
			frameData.RenderFinishedSemaphore = CreateRef<VulkanSemaphore>(mDevice, 0);

			frameData.UniformBuffer = CreateRef<VulkanBuffer>(mDevice, sizeof(UniformBufferObject), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
			frameData.UniformBuffer->GetDeviceMemory()->Map(0, sizeof(UniformBufferObject), &frameData.UniformBufferMappedData);

			frameData.DescriptorSet = descriptorSets[i];
			frameData.CommandBuffer = commandBuffers[i];

			Vector<VulkanDescriptorBinding> descriptorBindings = {
				VulkanDescriptorBinding {
					.Buffer  = frameData.UniformBuffer,
					.Binding = 0,
					.Type    = EShaderResourceType::UniformBuffer
				},
				VulkanDescriptorBinding {
					.Image   = mTextureImage,
					.Binding = 1,
					.Type    = EShaderResourceType::CombinedImageSampler
				}
			};

			frameData.DescriptorSet->SetDescriptorBindings(descriptorBindings);
		}
	}

	void VulkanModule::OnShutdown()
	{
	}

	void VulkanModule::OnUpdate(Dt dt)
	{
		// Acquire image from the swapchain
		// Record command buffer which draws the scene onto that image
		// Submit the recorded command buffer
		// Present swapchain image

		FrameData& frameData = mFramesInFlight[mCurrentFrameIndex];

		frameData.AcquireNextImageFence->Wait();

		mSwapchain->AcquireNextImage(frameData.ImageAvailableSemaphore, nullptr);

		if (mSwapchain->GetImageIndex() == -1)
			return;

		if (mFramebufferResized)
		{
			mCurrentFrameIndex = (mCurrentFrameIndex + 1) % sMaxFramesInFlight;
			mFramebuffers.clear();

			const Vector<VkImageView>& swapchainAttachments = mSwapchain->GetImageViewHandles();
			for (uint32 i = 0; i < swapchainAttachments.size(); i++)
				mFramebuffers[i] = CreateRef<VulkanFramebuffer>(mDevice, mRenderPass, Vector<VkImageView>({ swapchainAttachments[i], mDepthTextureImage->GetImageViewHandle() }));

			mFramebufferResized = false;
			return;
		}

		frameData.AcquireNextImageFence->Reset();
		frameData.CommandBuffer->Reset();

		UpdateUniformBuffer();

		frameData.CommandBuffer->Begin();

		frameData.CommandBuffer->BeginRenderPass(mRenderPass, mFramebuffers[mSwapchain->GetImageIndex()]);

		frameData.CommandBuffer->BindPipeline(mPipeline);
		frameData.CommandBuffer->BindVertexBuffer(mVertexBuffer);
		frameData.CommandBuffer->BindIndexBuffer(mIndexBuffer);
		frameData.CommandBuffer->SetViewport({ 0.0f, 0.0f }, { (float32)mSwapchain->GetExtent().width, (float32)mSwapchain->GetExtent().height });
		frameData.CommandBuffer->SetScissor({ 0, 0 }, { mSwapchain->GetExtent().width, mSwapchain->GetExtent().height });
		frameData.CommandBuffer->BindDescriptorSet(frameData.DescriptorSet, mPipeline);
		frameData.CommandBuffer->DrawIndexed(CS_ARRAY_SIZE(mIndices));

		frameData.CommandBuffer->EndRenderPass();

		frameData.CommandBuffer->End();

		VkResult queueSubmitResult = mGraphicsQueue->Submit({ frameData.CommandBuffer }, { frameData.ImageAvailableSemaphore }, { frameData.RenderFinishedSemaphore }, frameData.AcquireNextImageFence);

		if (queueSubmitResult == VK_ERROR_OUT_OF_DATE_KHR || queueSubmitResult == VK_SUBOPTIMAL_KHR)
		{
			mSwapchain->Recreate();
			return;
		}
		else if (queueSubmitResult != VK_SUCCESS)
		{
			VK_ERROR(queueSubmitResult, "vkQueueSubmit failed.");
		}

		VkResult queuePresentResult = mPresentQueue->Present(mSwapchain, { frameData.RenderFinishedSemaphore });

		if (queueSubmitResult == VK_ERROR_OUT_OF_DATE_KHR || queueSubmitResult == VK_SUBOPTIMAL_KHR || mFramebufferResized)
		{
			mSwapchain->Recreate();
			mFramebufferResized = false;

			return;
		}

		if (queuePresentResult != VK_SUCCESS)
		{
			VK_ERROR(queuePresentResult, "vkQueuePresent failed.");
		}

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

	void VulkanModule::UpdateUniformBuffer()
	{
		float32 aspectRatio = (float32)mSwapchain->GetExtent().width / mSwapchain->GetExtent().height;

		glm::mat4 view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		glm::mat4 proj = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 10.0f);
		proj[1][1] *= -1;

		mTransformMatrix      = glm::rotate(glm::mat4(1.0f), Time::GetTime().InSeconds(), glm::vec3(0.0f, 0.0f, 1.0f));
		mViewProjectionMatrix = proj * view;

		UniformBufferObject ubo;
		ubo.ViewProjectionMatrix = mViewProjectionMatrix;
		ubo.TransformMatrix      = mTransformMatrix;

		memcpy(mFramesInFlight[mCurrentFrameIndex].UniformBufferMappedData, &ubo, sizeof(UniformBufferObject));
	}

#if 0

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
#endif

}