#pragma once
//#include "VulkanDebugUtils.hpp"
#include <vulkan/vulkan.h>
#include "Renderer/Shader.hpp"

namespace Cosmic
{

	VkShaderStageFlags EShaderStagesToVkShaderStageFlags(BitFlags<EShaderStage> shaderStages);
	VkDescriptorType   EShaderResourceTypeToVkDescriptorType(EShaderResourceType type);

	class VulkanDevice;
	
	class VulkanShader : public Shader
	{
	public:
		VulkanShader(const ShaderInfo& info, const Ref<VulkanDevice>& device);
		~VulkanShader();

	public:
		const VkVertexInputBindingDescription&           GetVertexInputBindingDescription();
		const Vector<VkVertexInputAttributeDescription>& GetVertexInputAttributeDescriptions();
		const Vector<VkDescriptorSetLayout>&             GetDescriptorSetLayouts();

	public:
		VkShaderModule GetShaderModuleHandle() const { return mShaderModuleHandle; }

		void CreateShaderModuleHandle();
		void DestroyShaderModuleHandle();

		VkShaderModule GetOrCreateShaderModuleHandle();

	private:
		VkShaderModule mShaderModuleHandle = VK_NULL_HANDLE;

		VkVertexInputBindingDescription           mVertexInputBindingDescription;
		Vector<VkVertexInputAttributeDescription> mVertexInputAttributeDescriptions;
		Vector<VkDescriptorSetLayout>             mDescriptorSetLayouts;

		Ref<VulkanDevice> mDevice;
	};

}
