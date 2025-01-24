#pragma once
#include "VulkanInternal.hpp"

#include "Renderer/Framebuffer.hpp"

namespace Cosmic
{

    ETextureFormat VkFormatToETextureFormat(VkFormat format);
    VkFormat ETextureFormatToVkFormat(ETextureFormat format);

    class VulkanDevice;
    class VulkanRenderPass;

	class VulkanFramebuffer : public IRefCounted
	{
    public:
        VulkanFramebuffer(const Ref<VulkanDevice>& device, const Ref<VulkanRenderPass>& renderPass, const Vector<VkImageView>& attachmentHandles);

    public:
        VkFramebuffer GetHandle() const { return mHandle; }

        const Ref<VulkanRenderPass>& GetRenderPass() const { return mRenderPass; }

        const Vector<VkImageView>& GetAttachmentHandles() const { return mAttachmentHandles; }

    private:
        VkFramebuffer mHandle;

        Vector<VkImageView> mAttachmentHandles;

        Ref<VulkanRenderPass> mRenderPass;
        Ref<VulkanDevice> mDevice;
	};

}
