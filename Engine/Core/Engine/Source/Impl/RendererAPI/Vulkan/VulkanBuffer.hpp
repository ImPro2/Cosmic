#pragma once
#include "VulkanDebugUtils.hpp"
#include "VulkanDeviceMemory.hpp"

#include "Base/Buffer.hpp"
#include "Renderer/Buffer.hpp"

namespace Cosmic
{

    class VulkanDevice;

    class VulkanBuffer : public IRefCounted
    {
    public:
        VulkanBuffer(const Ref<VulkanDevice>& device, uint32 size, VkBufferUsageFlags usage, VkMemoryPropertyFlags memoryPropertyFlags);
        //VulkanBuffer(const Ref<VulkanDevice>& device, uint32 size, VkBufferUsageFlags usage, VkSharingMode sharingMode);
        ~VulkanBuffer();

    public:
        VkBuffer           GetHandle()     const { return mBufferHandle; }
        VkBufferUsageFlags GetUsageFlags() const { return mUsageFlags;   }

        Ref<VulkanDeviceMemory>        GetDeviceMemory()       { return mDeviceMemory; }
        const Ref<VulkanDeviceMemory>& GetDeviceMemory() const { return mDeviceMemory; }

    private:
        VkBuffer mBufferHandle;
        VkBufferUsageFlags mUsageFlags;

        Ref<VulkanDeviceMemory> mDeviceMemory;

        Ref<VulkanDevice> mDevice;
    };

#if 0
	class VulkanVertexBuffer : public VulkanBuffer
	{
    public:
        VulkanVertexBuffer(uint32 size);
        VulkanVertexBuffer(void* vertices, uint32 size, EBufferUsage usage);
        virtual ~VulkanVertexBuffer();

        void SetData(const void* data, uint32 size, uint32 offset = 0);

    public:
        VkBuffer GetHandle() const { return mBufferHandle; }
        
        Ref<VulkanDeviceMemory>        GetDeviceMemory()       { return mDeviceMemory; }
        const Ref<VulkanDeviceMemory>& GetDeviceMemory() const { return mDeviceMemory; }

    private:

        Ref<VulkanDevice> mDevice;
	};
#endif

}
