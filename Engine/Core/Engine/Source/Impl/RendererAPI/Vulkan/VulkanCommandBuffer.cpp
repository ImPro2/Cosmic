#include "cspch.hpp"
#include "VulkanCommandBuffer.hpp"
#include "VulkanDevice.hpp"
#include "VulkanCommandPool.hpp"

#include "VulkanRenderPass.hpp"
#include "VulkanFramebuffer.hpp"
#include "VulkanBuffer.hpp"
#include "VulkanGraphicsPipeline.hpp"
#include "VulkanDescriptorSet.hpp"
#include "VulkanImage.hpp"

CS_MODULE_LOG_INFO(Cosmic, Impl.RendererAPI.Vulkan.VulkanCommandBuffer);

namespace Cosmic
{

	namespace Utils
	{

		static Pair<VkAccessFlags, VkPipelineStageFlags> VkImageLayoutToVkAccesFlgasAndVkPipelineStageFlags(VkImageLayout imageLayout)
		{
			switch (imageLayout)
			{
				case VK_IMAGE_LAYOUT_UNDEFINED:                return { 0, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT };
				case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:     return { VK_ACCESS_TRANSFER_WRITE_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT };
				case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL: return { VK_ACCESS_SHADER_READ_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT };
			}

			CS_ASSERT(false, "Unsupported image layout transition");

			return {};
		}

	}

	VulkanCommandBuffer::VulkanCommandBuffer(const Ref<VulkanDevice>& device, const Ref<VulkanCommandPool>& commandPool, VkCommandBuffer commandBufferHandle, VkCommandBufferLevel level)
		: mDevice(device), mCommandPool(commandPool), mCommandBufferHandle(commandBufferHandle), mCommandBufferLevel(level)
	{
	}

	VulkanCommandBuffer::~VulkanCommandBuffer()
	{
		vkFreeCommandBuffers(mDevice->GetHandle(), mCommandPool->GetHandle(), 1, &mCommandBufferHandle);
	}

	void VulkanCommandBuffer::Reset(bool releaseResources)
	{
		VkCommandBufferResetFlags resetFlags = 0;
		if (releaseResources)
			resetFlags |= VK_COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT;

		VK_CALL(vkResetCommandBuffer(mCommandBufferHandle, resetFlags));
	}

	void VulkanCommandBuffer::Begin(bool singleTimeCommands)
	{
		VkCommandBufferBeginInfo commandBufferBeginInfo = {
			.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
			.flags = singleTimeCommands ? VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT : (VkCommandBufferUsageFlags)0,
		};

		VK_CALL(vkBeginCommandBuffer(mCommandBufferHandle, &commandBufferBeginInfo));
	}

	void VulkanCommandBuffer::End()
	{
		VK_CALL(vkEndCommandBuffer(mCommandBufferHandle));
	}
	
	void VulkanCommandBuffer::TransitionImageLayout(const Ref<VulkanImage>& image, VkImageLayout newLayout)
	{
		VkImageLayout oldLayout = image->GetImageLayout();
		image->mImageLayout = newLayout;

		auto [srcAccess, srcStage] = Utils::VkImageLayoutToVkAccesFlgasAndVkPipelineStageFlags(oldLayout);
		auto [dstAccess, dstStage] = Utils::VkImageLayoutToVkAccesFlgasAndVkPipelineStageFlags(newLayout);

		VkImageMemoryBarrier imageMemoryBarrier = {
			.sType               = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
			.srcAccessMask       = srcAccess,
			.dstAccessMask       = dstAccess,
			.oldLayout           = oldLayout,
			.newLayout           = newLayout,
			.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
			.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
			.image               = image->GetHandle(),
			.subresourceRange = {
				.aspectMask     = image->GetAspectFlags(),
				.baseMipLevel   = 0,
				.levelCount     = image->GetMipmapLevelCount(),
				.baseArrayLayer = 0,
				.layerCount     = 1
			}
		};

		vkCmdPipelineBarrier(mCommandBufferHandle, srcStage, dstStage, 0, 0, nullptr, 0, nullptr, 1, &imageMemoryBarrier);
	}

	void VulkanCommandBuffer::CopyBufferToImage(const Ref<VulkanBuffer>& buffer, const Ref<VulkanImage>& image, uint32 bufferOffset, uint2 imageOffset)
	{
		VkBufferImageCopy bufferImageCopy = {
			.bufferOffset      = bufferOffset,
			.bufferRowLength   = 0,
			.bufferImageHeight = 0,
			.imageSubresource = {
				.aspectMask     = image->GetAspectFlags(),
				.mipLevel       = 0,
				.baseArrayLayer = 0,
				.layerCount     = 1
			},
			.imageOffset = { (int32)imageOffset.x, (int32)imageOffset.y, 0 },
			.imageExtent = { image->GetSize().width, image->GetSize().height, 1 }
		};

		vkCmdCopyBufferToImage(mCommandBufferHandle, buffer->GetHandle(), image->GetHandle(), image->GetImageLayout(), 1, &bufferImageCopy);
	}

	void VulkanCommandBuffer::CopyBuffer(const Ref<VulkanBuffer>& srcBuffer, const Ref<VulkanBuffer>& dstBuffer, uint32 srcOffset, uint32 dstOffset)
	{
		VkBufferCopy bufferCopy = {
			.srcOffset = srcOffset,
			.dstOffset = dstOffset,
			.size      = srcBuffer->GetDeviceMemory()->GetMemoryRequirements().size
		};

		vkCmdCopyBuffer(mCommandBufferHandle, srcBuffer->GetHandle(), dstBuffer->GetHandle(), 1, &bufferCopy);
	}

	void VulkanCommandBuffer::BeginRenderPass(const Ref<VulkanRenderPass>& renderPass, const Ref<VulkanFramebuffer>& framebuffer, VkRect2D renderArea, VkSubpassContents subpassContents)
	{
		const Vector<AttachmentInfo>& colorAttachments       = renderPass->GetOutputColorAttachments();
		const AttachmentInfo&         depthStencilAttachment = renderPass->GetDepthStencilAttachment();

		uint32 clearValueCount = colorAttachments.size() + (depthStencilAttachment.Name != "" ? 1 : 0);

		VkClearValue* clearValues = (VkClearValue*)alloca(clearValueCount * sizeof(VkClearValue));

		for (uint32 i = 0; i < clearValueCount; i++)
		{
			if (i < colorAttachments.size())
			{
				float4 clearColor = colorAttachments[i].ClearValue.ClearColor;

				clearValues[i].color = { { clearColor.r, clearColor.g, clearColor.b, clearColor.a } };
			}
			else
			{
				float32 depth   = depthStencilAttachment.ClearValue.ClearDepth;
				uint32  stencil = depthStencilAttachment.ClearValue.ClearStencil;

				clearValues[i].depthStencil = { depth, stencil };
			}
		}

		if (renderArea.extent.width == 0 && renderArea.extent.height == 0)
		{
			renderArea.offset = { 0, 0 };
			renderArea.extent = { renderPass->GetSize().width, renderPass->GetSize().height };
		}

		VkRenderPassBeginInfo renderPassBeginInfo = {
			.sType           = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
			.renderPass      = renderPass->GetHandle(),
			.framebuffer     = framebuffer->GetHandle(),
			.renderArea      = renderArea,
			.clearValueCount = clearValueCount,
			.pClearValues    = clearValues
		};

		vkCmdBeginRenderPass(mCommandBufferHandle, &renderPassBeginInfo, subpassContents);
	}

	void VulkanCommandBuffer::EndRenderPass()
	{
		vkCmdEndRenderPass(mCommandBufferHandle);
	}

	void VulkanCommandBuffer::BindPipeline(const Ref<VulkanGraphicsPipeline>& graphicsPipeline)
	{
		vkCmdBindPipeline(mCommandBufferHandle, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline->GetHandle());
	}

	void VulkanCommandBuffer::BindVertexBuffer(const Ref<VulkanBuffer>& vertexBuffer, uint32 offset)
	{
		VkBuffer     vertexBufferHandle = vertexBuffer->GetHandle();
		VkDeviceSize deviceSizeOffset = offset;

		vkCmdBindVertexBuffers(mCommandBufferHandle, 0, 1, &vertexBufferHandle, &deviceSizeOffset);
	}

	void VulkanCommandBuffer::BindVertexBuffers(const Vector<Ref<VulkanBuffer>>& vertexBuffers, Vector<uint32> offsets /*= {}*/)
	{
		VkBuffer* vertexBufferHandles = (VkBuffer*)alloca(vertexBuffers.size() * sizeof(VkBuffer));

		for (uint32 i = 0; i < vertexBuffers.size(); i++)
		{
			vertexBufferHandles[i] = vertexBuffers[i]->GetHandle();
		}

		if (offsets.empty())
		{
			offsets.resize(vertexBuffers.size());
			for (uint32 i = 0; i < vertexBuffers.size(); i++)
			{
				offsets[i] = 0;
			}
		}

		vkCmdBindVertexBuffers(mCommandBufferHandle, 0, vertexBuffers.size(), vertexBufferHandles, (VkDeviceSize*)offsets.data());
	}

	void VulkanCommandBuffer::BindIndexBuffer(const Ref<VulkanBuffer>& indexBuffer, uint32 offset)
	{
		vkCmdBindIndexBuffer(mCommandBufferHandle, indexBuffer->GetHandle(), offset, VK_INDEX_TYPE_UINT16);
	}

	void VulkanCommandBuffer::BindDescriptorSet(const Ref<VulkanDescriptorSet>& descriptorSet, const Ref<VulkanGraphicsPipeline>& graphicsPipeline, uint32 dynamicOffset)
	{
		VkDescriptorSet descriptorSetHandle = descriptorSet->GetHandle();

		vkCmdBindDescriptorSets(mCommandBufferHandle, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline->GetPipelineLayoutHandle(), 0, 1, &descriptorSetHandle, dynamicOffset == 0 ? 0 : 1, dynamicOffset == 0 ? nullptr : &dynamicOffset);
	}

	void VulkanCommandBuffer::BindDescriptorSets(const Vector<Ref<VulkanDescriptorSet>>& descriptorSets, const Ref<VulkanGraphicsPipeline>& graphicsPipeline, const Vector<uint32>& dynamicOffsets /*= {}*/)
	{
		VkDescriptorSet* descriptorSetHandles = (VkDescriptorSet*)alloca(sizeof(VkDescriptorSet) * descriptorSets.size());

		for (uint32 i = 0; i < descriptorSets.size(); i++)
			descriptorSetHandles[i] = descriptorSets[i]->GetHandle();

		vkCmdBindDescriptorSets(mCommandBufferHandle, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline->GetPipelineLayoutHandle(), 0, descriptorSets.size(), descriptorSetHandles, dynamicOffsets.size(), dynamicOffsets.empty() ? nullptr : dynamicOffsets.data());
	}

	void VulkanCommandBuffer::SetViewport(float2 topLeft, float2 btmRight)
	{
		VkViewport viewport = {
			.x        = topLeft.x,
			.y        = topLeft.y,
			.width    = btmRight.x,
			.height   = btmRight.y,
			.minDepth = 0.0f,
			.maxDepth = 1.0f
		};

		vkCmdSetViewport(mCommandBufferHandle, 0, 1, &viewport);
	}

	void VulkanCommandBuffer::SetScissor(uint2 topLeft, uint2 btmRight)
	{
		VkRect2D scissorRect = {
			.offset = { (int32)topLeft.x, (int32)topLeft.y  },
			.extent = { btmRight.x, btmRight.y }
		};
		
		vkCmdSetScissor(mCommandBufferHandle, 0, 1, &scissorRect);
	}

	void VulkanCommandBuffer::DrawIndexed(uint32 indexCount)
	{
		vkCmdDrawIndexed(mCommandBufferHandle, indexCount, 1, 0, 0, 0);
	}

}