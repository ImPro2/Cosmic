#include "cspch.hpp"
#include "VulkanTexture.hpp"

#include "VulkanDevice.hpp"

namespace Cosmic 
{

	VulkanTexture2D::VulkanTexture2D(const Texture2DInfo& info, const Ref<VulkanDevice>& device)
		: Texture2D(info), mDevice(device)
	{
	}

	VulkanTexture2D::~VulkanTexture2D()
	{
		vkDestroyImage(mDevice->GetHandle(), mImageHandle, nullptr);
		vkDestroyImageView(mDevice->GetHandle(), mImageViewHandle, nullptr);
		vkFreeMemory(mDevice->GetHandle(), mImageMemoryHandle, nullptr);
	}

	void VulkanTexture2D::SetData(void* data, uint32 size)
	{

	}

	void VulkanTexture2D::Bind(uint32 slot /*= 0*/) const
	{

	}

	void VulkanTexture2D::CreateImageHandle()
	{

	}

	void VulkanTexture2D::AllocateImageMemory()
	{

	}

	void VulkanTexture2D::CreateImageView()
	{

	}

}
