#include "cspch.hpp"
#include "VulkanImage.hpp"
#include "VulkanDevice.hpp"
#include "VulkanPhysicalDevice.hpp"
#include "VulkanCommandBuffer.hpp"
#include "VulkanCommandPool.hpp"

namespace Cosmic
{
	VulkanImage::VulkanImage(const Ref<VulkanDevice>& device, uint2 size, uint32 mipLevels, VkFormat format, VkImageUsageFlags usage, VkMemoryPropertyFlags memoryPropertyFlags, VkImageAspectFlags aspectFlags, bool sampled)
		: mDevice(device), mSize(size), mMipLevels(mipLevels), mFormat(format), mImageAspectFlags(aspectFlags)
	{
		VkImageCreateInfo imageCreateInfo = {
			.sType                 = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
			.flags                 = 0,
			.imageType             = VK_IMAGE_TYPE_2D,
			.format                = format,
			.extent                = { size.x, size.y, 1 },
			.mipLevels             = mipLevels,
			.arrayLayers           = 1,
			.samples               = VK_SAMPLE_COUNT_1_BIT,
			.tiling                = VK_IMAGE_TILING_OPTIMAL,
			.usage                 = usage,
			.sharingMode           = VK_SHARING_MODE_EXCLUSIVE,
			.queueFamilyIndexCount = 0,
			.pQueueFamilyIndices   = nullptr,
			.initialLayout         = mImageLayout
		};

		VK_CALL(vkCreateImage(mDevice->GetHandle(), &imageCreateInfo, nullptr, &mImageHandle));

		VkMemoryRequirements memoryRequirements;
		vkGetImageMemoryRequirements(mDevice->GetHandle(), mImageHandle, &memoryRequirements);

		mDeviceMemory = CreateRef<VulkanDeviceMemory>(mDevice, memoryRequirements, memoryPropertyFlags);
		mDeviceMemory->Allocate();

		VK_CALL(vkBindImageMemory(mDevice->GetHandle(), mImageHandle, mDeviceMemory->GetHandle(), 0));

		VkImageViewCreateInfo imageViewCreateInfo = {
			.sType      = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
			.flags      = 0,
			.image      = mImageHandle,
			.viewType   = VK_IMAGE_VIEW_TYPE_2D,
			.format     = format,
			.components = {
				.r = VK_COMPONENT_SWIZZLE_IDENTITY,
				.g = VK_COMPONENT_SWIZZLE_IDENTITY,
				.b = VK_COMPONENT_SWIZZLE_IDENTITY,
				.a = VK_COMPONENT_SWIZZLE_IDENTITY,
			},
			.subresourceRange = {
				.aspectMask     = aspectFlags,
				.baseMipLevel   = 0,
				.levelCount     = mipLevels,
				.baseArrayLayer = 0,
				.layerCount     = 1
			}
		};

		VK_CALL(vkCreateImageView(mDevice->GetHandle(), &imageViewCreateInfo, nullptr, &mImageViewHandle));

		if (sampled)
		{
			VkPhysicalDeviceProperties physicalDeviceProperties;
			vkGetPhysicalDeviceProperties(mDevice->GetPhysicalDevice()->GetHandle(), &physicalDeviceProperties);

			VkSamplerCreateInfo samplerCreateInfo = {
				.sType                   = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,
				.flags                   = 0,
				.magFilter               = VK_FILTER_LINEAR,
				.minFilter               = VK_FILTER_LINEAR,
				.mipmapMode              = VK_SAMPLER_MIPMAP_MODE_LINEAR,
				.addressModeU            = VK_SAMPLER_ADDRESS_MODE_REPEAT,
				.addressModeV            = VK_SAMPLER_ADDRESS_MODE_REPEAT,
				.addressModeW            = VK_SAMPLER_ADDRESS_MODE_REPEAT,
				.mipLodBias              = 0.0f,
				.anisotropyEnable        = VK_TRUE,
				.maxAnisotropy           = physicalDeviceProperties.limits.maxSamplerAnisotropy,
				.compareEnable           = VK_FALSE,
				.compareOp               = {},
				.minLod                  = 0.0f,
				.maxLod                  = VK_LOD_CLAMP_NONE,
				.borderColor             = VK_BORDER_COLOR_INT_OPAQUE_BLACK,
				.unnormalizedCoordinates = VK_FALSE,
			};

			VK_CALL(vkCreateSampler(mDevice->GetHandle(), &samplerCreateInfo, nullptr, &mSamplerHandle));
		}
	}

	VulkanImage::~VulkanImage()
	{

	}

}