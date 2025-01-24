#pragma once
#include "VulkanDebugUtils.hpp"

#include "Renderer/Texture.hpp"

namespace Cosmic
{

    class VulkanDevice;

	class VulkanTexture2D : public Texture2D
	{
    public:
        VulkanTexture2D(const Texture2DInfo& info, const Ref<VulkanDevice>& device);
        virtual ~VulkanTexture2D();

    public:
        virtual void SetData(void* data, uint32 size);
        virtual void Bind(uint32 slot = 0) const;

    public:
        VkImage        GetImageHandle()       const { return mImageHandle;       }
        VkDeviceMemory GetImageMemoryHandle() const { return mImageMemoryHandle; }
        VkImageView    GetImageViewHandle()   const { return mImageViewHandle;   }

    private:
        void CreateImageHandle();
        void AllocateImageMemory();
        void CreateImageView();

    private:
        VkImage        mImageHandle;
        VkDeviceMemory mImageMemoryHandle;
        VkImageView    mImageViewHandle;

        Ref<VulkanDevice> mDevice;
	};

}
