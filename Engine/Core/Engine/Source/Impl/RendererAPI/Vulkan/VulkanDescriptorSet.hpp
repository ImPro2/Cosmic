#pragma once
#include "VulkanDebugUtils.hpp"

#include "Renderer/ShaderReflectionData.hpp"
#include "Memory/Memory.hpp"

namespace Cosmic
{

	class VulkanDevice;
	class VulkanDescriptorPool;
	class VulkanBuffer;
	class VulkanImage;

	struct VulkanDescriptorBinding
	{
		// Either buffer or image set, not both
		WeakRef<VulkanBuffer>   Buffer;
		WeakRef<VulkanImage>    Image;
		uint32              Binding;
		EShaderResourceType Type;
	};

	class VulkanDescriptorSet : public IRefCounted
	{
	public:
		VulkanDescriptorSet(const Ref<VulkanDevice>& device, const Ref<VulkanDescriptorPool>& descriptorPool, VkDescriptorSet handle);
		~VulkanDescriptorSet();

	public:
		void SetDescriptorBindings(const Vector<VulkanDescriptorBinding>& bindings);

	public:
		VkDescriptorSet GetHandle() const { return mHandle; }

	private:
		VkDescriptorSet mHandle = VK_NULL_HANDLE;

		Ref<VulkanDescriptorPool> mDescriptorPool;
		Ref<VulkanDevice> mDevice;
	};

}
