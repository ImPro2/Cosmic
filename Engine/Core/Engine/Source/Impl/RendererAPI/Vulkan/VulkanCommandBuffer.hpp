#pragma once
#include "VulkanDebugUtils.hpp"

namespace Cosmic
{

	class VulkanDevice;
	class VulkanCommandPool;

	class VulkanRenderPass;
	class VulkanFramebuffer;
	class VulkanBuffer;
	class VulkanGraphicsPipeline;
	class VulkanDescriptorSet;
	class VulkanImage;

	class VulkanCommandBuffer : public IRefCounted
	{
	public:
		VulkanCommandBuffer(const Ref<VulkanDevice>& device, const Ref<VulkanCommandPool>& commandPool, VkCommandBuffer commandBufferHandle, VkCommandBufferLevel level);
		~VulkanCommandBuffer();

	public:
		void Reset(bool releaseResources = false);

		void Begin(bool singleTimeCommands = false);
		void End();

	public:
		void TransitionImageLayout(const Ref<VulkanImage>& image, VkImageLayout newLayout);
		void CopyBufferToImage(const Ref<VulkanBuffer>& buffer, const Ref<VulkanImage>& image, uint32 bufferOffset = 0, uint2 imageOffset = { 0, 0 });

		void CopyBuffer(const Ref<VulkanBuffer>& srcBuffer, const Ref<VulkanBuffer>& dstBuffer, uint32 srcOffset = 0, uint32 dstOffset = 0);

		void BeginRenderPass(const Ref<VulkanRenderPass>& renderPass, const Ref<VulkanFramebuffer>& framebuffer, VkRect2D renderArea = { 0, 0 }, VkSubpassContents subpassContents = VK_SUBPASS_CONTENTS_INLINE);
		void EndRenderPass();

		void BindPipeline(const Ref<VulkanGraphicsPipeline>& graphicsPipeline);
		void BindVertexBuffer(const Ref<VulkanBuffer>& vertexBuffer, uint32 offset = 0);
		void BindVertexBuffers(const Vector<Ref<VulkanBuffer>>& vertexBuffers, Vector<uint32> offsets = {});
		void BindIndexBuffer(const Ref<VulkanBuffer>& indexBuffer, uint32 offset = 0);
		void BindDescriptorSet(const Ref<VulkanDescriptorSet>& descriptorSet, const Ref<VulkanGraphicsPipeline>& graphicsPipeline, uint32 dynamicOffset = 0);
		void BindDescriptorSets(const Vector<Ref<VulkanDescriptorSet>>& descriptorSets, const Ref<VulkanGraphicsPipeline>& graphicsPipeline, const Vector<uint32>& dynamicOffsets = {});

		void SetViewport(float2 topLeft, float2 btmRight);
		void SetScissor(uint2 topLeft, uint2 btmRight);

		void DrawIndexed(uint32 indexCount);

	public:
		VkCommandBuffer*     GetHandlePtr()    { return &mCommandBufferHandle; }
		VkCommandBuffer      GetHandle() const { return mCommandBufferHandle;  }
		VkCommandBufferLevel GetLevel()  const { return mCommandBufferLevel;   }

		const Ref<VulkanCommandPool>& GetCommandPool() const { return mCommandPool; }
		Ref<VulkanCommandPool>        GetCommandPool()       { return mCommandPool; }

	private:
		VkCommandBuffer      mCommandBufferHandle;
		VkCommandBufferLevel mCommandBufferLevel;

		Ref<VulkanCommandPool> mCommandPool;
		Ref<VulkanDevice> mDevice;

		friend class VulkanCommandPool;
	};

}
