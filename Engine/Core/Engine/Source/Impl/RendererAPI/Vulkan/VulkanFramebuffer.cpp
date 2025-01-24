#include "cspch.hpp"
#include "VulkanFramebuffer.hpp"
#include "VulkanDevice.hpp"
#include "VulkanRenderPass.hpp"

CS_MODULE_LOG_INFO(Cosmic, Impl.RendererAPI.Vulkan.VulkanFramebuffer);

namespace Cosmic
{
	ETextureFormat VkFormatToETextureFormat(VkFormat format)
	{
		switch (format)
		{
			case VK_FORMAT_R8_UNORM:            return ETextureFormat::R8_UNorm;
			case VK_FORMAT_R8_SNORM:            return ETextureFormat::R8_SNorm;
			case VK_FORMAT_R8_UINT:             return ETextureFormat::R8_UInt;
			case VK_FORMAT_R8_SINT:             return ETextureFormat::R8_SInt;
			case VK_FORMAT_R8_SRGB:             return ETextureFormat::R8_SRGB;
			case VK_FORMAT_R8G8_UNORM:          return ETextureFormat::RG8_UNorm;
			case VK_FORMAT_R8G8_SNORM:          return ETextureFormat::RG8_SNorm;
			case VK_FORMAT_R8G8_UINT:           return ETextureFormat::RG8_UInt;
			case VK_FORMAT_R8G8_SINT:           return ETextureFormat::RG8_SInt;
			case VK_FORMAT_R8G8_SRGB:           return ETextureFormat::RG8_SRGB;
			case VK_FORMAT_R8G8B8_UNORM:        return ETextureFormat::RGB8_UNorm;
			case VK_FORMAT_R8G8B8_SNORM:        return ETextureFormat::RGB8_SNorm;
			case VK_FORMAT_R8G8B8_UINT:         return ETextureFormat::RGB8_UInt;
			case VK_FORMAT_R8G8B8_SINT:         return ETextureFormat::RGB8_SInt;
			case VK_FORMAT_R8G8B8_SRGB:         return ETextureFormat::RGB8_SRGB;
			case VK_FORMAT_B8G8R8_UNORM:        return ETextureFormat::BGR8_UNorm;
			case VK_FORMAT_B8G8R8_SNORM:        return ETextureFormat::BGR8_SNorm;
			case VK_FORMAT_B8G8R8_UINT:         return ETextureFormat::BGR8_UInt;
			case VK_FORMAT_B8G8R8_SINT:         return ETextureFormat::BGR8_SInt;
			case VK_FORMAT_B8G8R8_SRGB:         return ETextureFormat::BGR8_SRGB;
			case VK_FORMAT_R8G8B8A8_UNORM:      return ETextureFormat::RGBA8_UNorm;
			case VK_FORMAT_R8G8B8A8_SNORM:      return ETextureFormat::RGBA8_SNorm;
			case VK_FORMAT_R8G8B8A8_UINT:       return ETextureFormat::RGB8A_UInt;
			case VK_FORMAT_R8G8B8A8_SINT:       return ETextureFormat::RGBA8_SInt;
			case VK_FORMAT_R8G8B8A8_SRGB:       return ETextureFormat::RGBA8_SRGB;
			case VK_FORMAT_B8G8R8A8_UNORM:      return ETextureFormat::BGRA8_UNorm;
			case VK_FORMAT_B8G8R8A8_SNORM:      return ETextureFormat::BGRA8_SNorm;
			case VK_FORMAT_B8G8R8A8_UINT:       return ETextureFormat::BGRA8_UInt;
			case VK_FORMAT_B8G8R8A8_SINT:       return ETextureFormat::BGRA8_SInt;
			case VK_FORMAT_B8G8R8A8_SRGB:       return ETextureFormat::BGRA8_SRGB;
			case VK_FORMAT_R16_UNORM:           return ETextureFormat::R16_UNorm;
			case VK_FORMAT_R16_SNORM:           return ETextureFormat::R16_SNorm;
			case VK_FORMAT_R16_UINT:            return ETextureFormat::R16_UInt;
			case VK_FORMAT_R16_SINT:            return ETextureFormat::R16_SInt;
			case VK_FORMAT_R16_SFLOAT:          return ETextureFormat::R16_SFloat;
			case VK_FORMAT_R16G16_UNORM:        return ETextureFormat::RG16_UNorm;
			case VK_FORMAT_R16G16_SNORM:        return ETextureFormat::RG16_SNorm;
			case VK_FORMAT_R16G16_UINT:         return ETextureFormat::RG16_UInt;
			case VK_FORMAT_R16G16_SINT:         return ETextureFormat::RG16_SInt;
			case VK_FORMAT_R16G16_SFLOAT:       return ETextureFormat::RG16_SFloat;
			case VK_FORMAT_R16G16B16_UNORM:     return ETextureFormat::RGB16_UNorm;
			case VK_FORMAT_R16G16B16_SNORM:     return ETextureFormat::RGB16_SNorm;
			case VK_FORMAT_R16G16B16_UINT:      return ETextureFormat::RGB16_UInt;
			case VK_FORMAT_R16G16B16_SINT:      return ETextureFormat::RGB16_SInt;
			case VK_FORMAT_R16G16B16_SFLOAT:    return ETextureFormat::RGB16_SFloat;
			case VK_FORMAT_R16G16B16A16_UNORM:  return ETextureFormat::RGBA16_UNorm;
			case VK_FORMAT_R16G16B16A16_SNORM:  return ETextureFormat::RGBA16_SNorm;
			case VK_FORMAT_R16G16B16A16_UINT:   return ETextureFormat::RGB16A_UInt;
			case VK_FORMAT_R16G16B16A16_SINT:   return ETextureFormat::RGBA16_SInt;
			case VK_FORMAT_R16G16B16A16_SFLOAT: return ETextureFormat::RGBA16_SFloat;
			case VK_FORMAT_R32_UINT:            return ETextureFormat::R32_UInt;
			case VK_FORMAT_R32_SINT:            return ETextureFormat::R32_SInt;
			case VK_FORMAT_R32_SFLOAT:          return ETextureFormat::R32_SFloat;
			case VK_FORMAT_R32G32_UINT:         return ETextureFormat::RG32_UInt;
			case VK_FORMAT_R32G32_SINT:         return ETextureFormat::RG32_SInt;
			case VK_FORMAT_R32G32_SFLOAT:       return ETextureFormat::RG32_SFloat;
			case VK_FORMAT_R32G32B32_UINT:      return ETextureFormat::RGB32_UInt;
			case VK_FORMAT_R32G32B32_SINT:      return ETextureFormat::RGB32_SInt;
			case VK_FORMAT_R32G32B32_SFLOAT:    return ETextureFormat::RGB32_SFloat;
			case VK_FORMAT_R32G32B32A32_UINT:   return ETextureFormat::RGB32A_UInt;
			case VK_FORMAT_R32G32B32A32_SINT:   return ETextureFormat::RGBA32_SInt;
			case VK_FORMAT_R32G32B32A32_SFLOAT: return ETextureFormat::RGBA32_SFloat;
			case VK_FORMAT_D16_UNORM:           return ETextureFormat::Depth16_UNorm;
			case VK_FORMAT_D32_SFLOAT:          return ETextureFormat::Depth32_SFloat;
			case VK_FORMAT_D16_UNORM_S8_UINT:   return ETextureFormat::Depth16_UNorm_Stencil8_UInt;
			case VK_FORMAT_D24_UNORM_S8_UINT:   return ETextureFormat::Depth24_UNorm_Stencil8_UInt;
			case VK_FORMAT_D32_SFLOAT_S8_UINT:  return ETextureFormat::Depth32_SFloat_Stencil8_UInt;
		}
	}

    VkFormat ETextureFormatToVkFormat(ETextureFormat format)
	{
        switch (format)
        {
			case ETextureFormat::R8_UNorm:                      return VK_FORMAT_R8_UNORM;
			case ETextureFormat::R8_SNorm:                      return VK_FORMAT_R8_SNORM;
			case ETextureFormat::R8_UInt:                       return VK_FORMAT_R8_UINT;
			case ETextureFormat::R8_SInt:                       return VK_FORMAT_R8_SINT;
			case ETextureFormat::R8_SRGB:                       return VK_FORMAT_R8_SRGB;
			case ETextureFormat::RG8_UNorm:                     return VK_FORMAT_R8G8_UNORM;
			case ETextureFormat::RG8_SNorm:                     return VK_FORMAT_R8G8B8_SNORM;
			case ETextureFormat::RG8_UInt:                      return VK_FORMAT_R8G8_UINT;
			case ETextureFormat::RG8_SInt:                      return VK_FORMAT_R8G8_SINT;
			case ETextureFormat::RG8_SRGB:                      return VK_FORMAT_R8G8_SRGB;
			case ETextureFormat::RGB8_UNorm:                    return VK_FORMAT_R8G8B8_UNORM;
			case ETextureFormat::RGB8_SNorm:                    return VK_FORMAT_R8G8B8_SNORM;
			case ETextureFormat::RGB8_UInt:                     return VK_FORMAT_R8G8B8_UINT;
			case ETextureFormat::RGB8_SInt:                     return VK_FORMAT_R8G8B8_SINT;
			case ETextureFormat::RGB8_SRGB:                     return VK_FORMAT_R8G8B8_SRGB;
			case ETextureFormat::BGR8_UNorm:                    return VK_FORMAT_B8G8R8_UNORM;
			case ETextureFormat::BGR8_SNorm:                    return VK_FORMAT_B8G8R8_SNORM;
			case ETextureFormat::BGR8_UInt:                     return VK_FORMAT_B8G8R8_UINT;
			case ETextureFormat::BGR8_SInt:                     return VK_FORMAT_B8G8R8_SINT;
			case ETextureFormat::BGR8_SRGB:                     return VK_FORMAT_B8G8R8_SRGB;
			case ETextureFormat::RGBA8_UNorm:                   return VK_FORMAT_R8G8B8A8_UNORM;
			case ETextureFormat::RGBA8_SNorm:                   return VK_FORMAT_R8G8B8A8_SNORM;
			case ETextureFormat::RGB8A_UInt:                    return VK_FORMAT_R8G8B8A8_UINT;
			case ETextureFormat::RGBA8_SInt:                    return VK_FORMAT_R8G8B8A8_SINT;
			case ETextureFormat::RGBA8_SRGB:                    return VK_FORMAT_R8G8B8A8_SRGB;
			case ETextureFormat::BGRA8_UNorm:                   return VK_FORMAT_B8G8R8A8_UNORM;
			case ETextureFormat::BGRA8_SNorm:                   return VK_FORMAT_B8G8R8A8_SNORM;
			case ETextureFormat::BGRA8_UInt:                    return VK_FORMAT_B8G8R8A8_UINT;
			case ETextureFormat::BGRA8_SInt:                    return VK_FORMAT_B8G8R8A8_SINT;
			case ETextureFormat::BGRA8_SRGB:                    return VK_FORMAT_B8G8R8A8_SRGB;
			case ETextureFormat::R16_UNorm:                     return VK_FORMAT_R16_UNORM;
			case ETextureFormat::R16_SNorm:                     return VK_FORMAT_R16_SNORM;
			case ETextureFormat::R16_UInt:                      return VK_FORMAT_R16_UINT;
			case ETextureFormat::R16_SInt:                      return VK_FORMAT_R16_SINT;
			case ETextureFormat::R16_SFloat:                    return VK_FORMAT_R16_SFLOAT;
			case ETextureFormat::RG16_UNorm:                    return VK_FORMAT_R16G16_UNORM;
			case ETextureFormat::RG16_SNorm:                    return VK_FORMAT_R16G16_SNORM;
			case ETextureFormat::RG16_UInt:                     return VK_FORMAT_R16G16_UINT;
			case ETextureFormat::RG16_SInt:                     return VK_FORMAT_R16G16_SINT;
			case ETextureFormat::RG16_SFloat:                   return VK_FORMAT_R16G16_SFLOAT;
			case ETextureFormat::RGB16_UNorm:                   return VK_FORMAT_R16G16B16_UNORM;
			case ETextureFormat::RGB16_SNorm:                   return VK_FORMAT_R16G16B16_SNORM;
			case ETextureFormat::RGB16_UInt:                    return VK_FORMAT_R16G16B16_UINT;
			case ETextureFormat::RGB16_SInt:                    return VK_FORMAT_R16G16B16_SINT;
			case ETextureFormat::RGB16_SFloat:                  return VK_FORMAT_R16G16B16_SFLOAT;
			case ETextureFormat::RGBA16_UNorm:                  return VK_FORMAT_R16G16B16A16_UNORM;
			case ETextureFormat::RGBA16_SNorm:                  return VK_FORMAT_R16G16B16A16_SNORM;
			case ETextureFormat::RGB16A_UInt:                   return VK_FORMAT_R16G16B16A16_UINT;
			case ETextureFormat::RGBA16_SInt:                   return VK_FORMAT_R16G16B16A16_SINT;
			case ETextureFormat::RGBA16_SFloat:                 return VK_FORMAT_R16G16B16A16_SFLOAT;
			case ETextureFormat::R32_UInt:                      return VK_FORMAT_R32_UINT;
			case ETextureFormat::R32_SInt:                      return VK_FORMAT_R32_SINT;
			case ETextureFormat::R32_SFloat:                    return VK_FORMAT_R32_SFLOAT;
			case ETextureFormat::RG32_UInt:                     return VK_FORMAT_R32G32_UINT;
			case ETextureFormat::RG32_SInt:                     return VK_FORMAT_R32G32_SINT;
			case ETextureFormat::RG32_SFloat:                   return VK_FORMAT_R32G32_SFLOAT;
			case ETextureFormat::RGB32_UInt:                    return VK_FORMAT_R32G32B32_UINT;
			case ETextureFormat::RGB32_SInt:                    return VK_FORMAT_R32G32B32_SINT;
			case ETextureFormat::RGB32_SFloat:                  return VK_FORMAT_R32G32B32_SFLOAT;
			case ETextureFormat::RGB32A_UInt:                   return VK_FORMAT_R32G32B32A32_UINT;
			case ETextureFormat::RGBA32_SInt:                   return VK_FORMAT_R32G32B32A32_SINT;
			case ETextureFormat::RGBA32_SFloat:                 return VK_FORMAT_R32G32B32A32_SFLOAT;
			case ETextureFormat::Depth16_UNorm:                 return VK_FORMAT_D16_UNORM;
			case ETextureFormat::Depth32_SFloat:                return VK_FORMAT_D32_SFLOAT;
			case ETextureFormat::Depth16_UNorm_Stencil8_UInt:   return VK_FORMAT_D16_UNORM_S8_UINT;
			case ETextureFormat::Depth24_UNorm_Stencil8_UInt:   return VK_FORMAT_D24_UNORM_S8_UINT;
			case ETextureFormat::Depth32_SFloat_Stencil8_UInt:  return VK_FORMAT_D32_SFLOAT_S8_UINT;
        }
	}

	VulkanFramebuffer::VulkanFramebuffer(const Ref<VulkanDevice>& device, const Ref<VulkanRenderPass>& renderPass, const Vector<VkImageView>& attachmentHandles)
		: mDevice(device), mRenderPass(renderPass), mAttachmentHandles(attachmentHandles)
	{
		VkFramebufferCreateInfo framebufferCreateInfo = {
			.sType           = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
			.flags           = 0,
			.renderPass      = mRenderPass->GetHandle(),
			.attachmentCount = (uint32)mAttachmentHandles.size(),
			.pAttachments    = mAttachmentHandles.data(),
			.width           = mRenderPass->GetSize().width,
			.height          = mRenderPass->GetSize().height,
			.layers          = 1
		};

		VK_CALL(vkCreateFramebuffer(mDevice->GetHandle(), &framebufferCreateInfo, nullptr, &mHandle));
	}

}