#include "cspch.hpp"
#include "VulkanRenderPass.hpp"
#include "VulkanFramebuffer.hpp"
#include "VulkanSwapchain.hpp"
#include "VulkanSurface.hpp"

#include "VulkanDevice.hpp"

namespace Cosmic
{

	VulkanRenderPass::VulkanRenderPass(const String& name, const Ref<VulkanSwapchain>& swapchain, const Ref<VulkanDevice>& device)
		: RenderPass(name), mSwapchain(swapchain), mDevice(device)
	{
	}

	VulkanRenderPass::~VulkanRenderPass()
	{
		vkDestroyRenderPass(mDevice->GetHandle(), mRenderPassHandle, nullptr);
	}

	void VulkanRenderPass::CreateHandle()
	{
		Vector<VkAttachmentDescription> attachmentDescriptions;
		Vector<VkSubpassDescription>    subpassDescriptions;
		Vector<VkSubpassDependency>     subpassDependencies;

		for (AttachmentInfo& attachmentInfo : mColorOutputs)
		{
			VkAttachmentDescription attachmentDescription = {
				.flags          = 0,
				.format         = ETextureFormatToVkFormat(attachmentInfo.Format),
				.samples        = VK_SAMPLE_COUNT_1_BIT,
				.stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
				.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
			};

			if (attachmentInfo.Flags.IsSet(EAttachmentFlags::SwapchainTarget))
			{
				attachmentDescription.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
				attachmentDescription.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
				attachmentDescription.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
				attachmentDescription.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

				if (attachmentInfo.Format == ETextureFormat::Unknown)
				{
					attachmentDescription.format = mSwapchain->GetSurfaceFormat().format;
				}

				if (attachmentInfo.Size.width == 0 && attachmentInfo.Size.height == 0)
				{
					attachmentInfo.Size = { mSwapchain->GetExtent().width, mSwapchain->GetExtent().height };
				}
			}

			attachmentDescriptions.push_back(attachmentDescription);
		}

		if (mDepthStencilOutput.Name != "")
		{
			VkAttachmentDescription attachmentDescription = {
				.flags          = 0,
				.format         = ETextureFormatToVkFormat(mDepthStencilOutput.Format),
				.samples        = VK_SAMPLE_COUNT_1_BIT,
				.loadOp         = VK_ATTACHMENT_LOAD_OP_CLEAR,
				.storeOp        = VK_ATTACHMENT_STORE_OP_DONT_CARE,
				.stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
				.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
				.initialLayout  = VK_IMAGE_LAYOUT_UNDEFINED,
				.finalLayout    = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL
			};

			attachmentDescriptions.push_back(attachmentDescription);
		}

		Vector<VkAttachmentReference> colorAttachmentReferences(mColorOutputs.size());

		for (uint32 i = 0; i < colorAttachmentReferences.size(); i++)
		{
			colorAttachmentReferences[i] = {
				.attachment = i,
				.layout     = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
			};
		}

		VkAttachmentReference depthStencilAttachmentReference = {
			.attachment = (uint32)attachmentDescriptions.size() - 1,
			.layout     = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL
		};

		VkSubpassDescription subpassDescription = {
			.flags                   = 0,
			.pipelineBindPoint       = VK_PIPELINE_BIND_POINT_GRAPHICS,
			.inputAttachmentCount    = 0,
			.pInputAttachments       = nullptr,
			.colorAttachmentCount    = (uint32)colorAttachmentReferences.size(),
			.pColorAttachments       = colorAttachmentReferences.data(),
			.pResolveAttachments     = nullptr,
			.pDepthStencilAttachment = &depthStencilAttachmentReference,
			.preserveAttachmentCount = 0,
			.pPreserveAttachments    = nullptr
		};

		subpassDescriptions.push_back(subpassDescription);

		VkSubpassDependency subpassDependency = {
			.srcSubpass      = VK_SUBPASS_EXTERNAL,
			.dstSubpass      = 0,
			.srcStageMask    = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT,
			.dstStageMask    = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT,
			.srcAccessMask   = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT,
			.dstAccessMask   = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT,
			.dependencyFlags = 0
		};

		subpassDependencies.push_back(subpassDependency);

		VkRenderPassCreateInfo renderPassCreateInfo = {
			.sType           = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
			.flags           = 0,
			.attachmentCount = (uint32)attachmentDescriptions.size(),
			.pAttachments    = attachmentDescriptions.data(),
			.subpassCount    = (uint32)subpassDescriptions.size(),
			.pSubpasses      = subpassDescriptions.data(),
			.dependencyCount = (uint32)subpassDependencies.size(),
			.pDependencies   = subpassDependencies.data()
		};

		VK_CALL(vkCreateRenderPass(mDevice->GetHandle(), &renderPassCreateInfo, nullptr, &mRenderPassHandle));
	}

	VkRenderPass VulkanRenderPass::GetOrCreateHandle()
	{
		if (mRenderPassHandle == VK_NULL_HANDLE)
			CreateHandle();

		return mRenderPassHandle;
	}

}