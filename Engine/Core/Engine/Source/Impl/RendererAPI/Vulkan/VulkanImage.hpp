#pragma once
#include "VulkanDebugUtils.hpp"
#include "VulkanDeviceMemory.hpp"

namespace Cosmic
{

	class VulkanDevice;
	class VulkanCommandBuffer;

	class VulkanImage : public IRefCounted
	{
	public:
		VulkanImage(const Ref<VulkanDevice>& device, uint2 size, uint32 mipLevels, VkFormat format, VkImageUsageFlags usage, VkMemoryPropertyFlags memoryPropertyFlags, VkImageAspectFlags aspectFlags, bool sampled);
		~VulkanImage();

	public:
		VkImage     GetHandle()          const { return mImageHandle;     }
		VkImageView GetImageViewHandle() const { return mImageViewHandle; }
		VkSampler   GetSamplerHandle()   const { return mSamplerHandle;   }
		
		uint2 GetSize() const { return mSize; }
		uint32 GetMipmapLevelCount() const { return mMipLevels; }

		VkFormat GetFormat() const { return mFormat; }
		VkImageAspectFlags GetAspectFlags() const { return mImageAspectFlags; }
		VkImageLayout GetImageLayout() const { return mImageLayout; }

        Ref<VulkanDeviceMemory>        GetDeviceMemory()       { return mDeviceMemory; }
        const Ref<VulkanDeviceMemory>& GetDeviceMemory() const { return mDeviceMemory; }

	private:
		VkImage     mImageHandle     = VK_NULL_HANDLE;
		VkImageView mImageViewHandle = VK_NULL_HANDLE;
		VkSampler   mSamplerHandle   = VK_NULL_HANDLE;

		uint2    mSize;
		uint32   mMipLevels;
		VkFormat mFormat;
		VkImageAspectFlags mImageAspectFlags;

		VkImageLayout mImageLayout = VK_IMAGE_LAYOUT_UNDEFINED;

		Ref<VulkanDeviceMemory> mDeviceMemory;
		Ref<VulkanDevice> mDevice;

		friend class VulkanCommandBuffer;
	};

}
