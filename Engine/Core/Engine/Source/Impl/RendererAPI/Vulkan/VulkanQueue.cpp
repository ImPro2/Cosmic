#include "cspch.hpp"
#include "VulkanQueue.hpp"

#include "VulkanDevice.hpp"
#include "VulkanSemaphore.hpp"
#include "VulkanFence.hpp"
#include "VulkanCommandBuffer.hpp"
#include "VulkanSwapchain.hpp"
#include "VulkanCommandPool.hpp"

CS_MODULE_LOG_INFO(Cosmic, Impl.RendererAPI.Vulkan.VulkanQueue);

namespace Cosmic
{

	VulkanQueue::VulkanQueue(const Ref<VulkanDevice>& device, VkQueue queueHandle, VkQueueFlags flags, int32 queueFamilyIndex, int32 queueIndex)
		: mDevice(device), mQueueHandle(queueHandle), mQueueFlags(flags), mQueueFamilyIndex(queueFamilyIndex), mQueueIndex(queueIndex)
	{
	}

	VulkanQueue::~VulkanQueue()
	{
	}

	void VulkanQueue::WaitIdle()
	{
		VK_CALL(vkQueueWaitIdle(mQueueHandle));
	}

	VkResult VulkanQueue::Submit(const Vector<Ref<VulkanCommandBuffer>>& commandBuffers, const Vector<Ref<VulkanSemaphore>>& waitSemaphores, const Vector<Ref<VulkanSemaphore>>& signalSemaphores, const Ref<VulkanFence>& signalFence)
	{
		VkCommandBuffer*      commandBufferHandles   = (VkCommandBuffer*)alloca(sizeof(VkCommandBuffer) * commandBuffers.size());
		VkSemaphore*          waitSemaphoreHandles   = (VkSemaphore*)alloca(sizeof(VkSemaphore) * waitSemaphores.size());
		VkPipelineStageFlags* pipelineStageFlags     = (VkPipelineStageFlags*)alloca(sizeof(VkPipelineStageFlags) * waitSemaphores.size());
		VkSemaphore*          signalSemaphoreHandles = (VkSemaphore*)alloca(sizeof(VkSemaphore) * signalSemaphores.size());

		VkFence signalFenceHandle = signalFence ? signalFence->GetHandle() : VK_NULL_HANDLE;

		for (uint32 i = 0; i < commandBuffers.size(); i++)
			commandBufferHandles[i] = commandBuffers[i]->GetHandle();

		for (uint32 i = 0; i < waitSemaphores.size(); i++)
		{
			waitSemaphoreHandles[i] = waitSemaphores[i]->GetHandle();
			pipelineStageFlags[i]   = waitSemaphores[i]->GetPipelineStageFlags();
		}

		for (uint32 i = 0; i < signalSemaphores.size(); i++)
			signalSemaphoreHandles[i] = signalSemaphores[i]->GetHandle();

		VkSubmitInfo submitInfo = {
			.sType                = VK_STRUCTURE_TYPE_SUBMIT_INFO,
			.waitSemaphoreCount   = (uint32)waitSemaphores.size(),
			.pWaitSemaphores      = waitSemaphoreHandles,
			.pWaitDstStageMask    = pipelineStageFlags,
			.commandBufferCount   = (uint32)commandBuffers.size(),
			.pCommandBuffers      = commandBufferHandles,
			.signalSemaphoreCount = (uint32)signalSemaphores.size(),
			.pSignalSemaphores    = signalSemaphoreHandles
		};

		return vkQueueSubmit(mQueueHandle, 1, &submitInfo, signalFenceHandle);
	}

	VkResult VulkanQueue::Present(const Ref<VulkanSwapchain>& swapchain, const Vector<Ref<VulkanSemaphore>>& waitSemaphores)
	{
		VkSemaphore* waitSemaphoreHandles = (VkSemaphore*)alloca(sizeof(VkSemaphore) * waitSemaphores.size());

		for (uint32 i = 0; i < waitSemaphores.size(); i++)
			waitSemaphoreHandles[i] = waitSemaphores[i]->GetHandle();

		uint32 imageIndices[] = { swapchain->GetImageIndex() };

		VkPresentInfoKHR presentInfo = {
			.sType              = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
			.waitSemaphoreCount = (uint32)waitSemaphores.size(),
			.pWaitSemaphores    = waitSemaphoreHandles,
			.swapchainCount     = 1,
			.pSwapchains        = swapchain->GetHandlePtr(),
			.pImageIndices      = imageIndices,
			.pResults           = nullptr,
		};

		return vkQueuePresentKHR(mQueueHandle, &presentInfo);
	}

	void VulkanQueue::SubmitSingleTimeCommands(const Ref<VulkanCommandPool>& commandPool, const Ref<VulkanFence>& signalFence, std::function<void(const Ref<VulkanCommandBuffer>&)> function, uint64 timeout)
	{
		Ref<VulkanCommandBuffer> commandBuffer = commandPool->AllocateCommandBuffer();

		commandBuffer->Begin(true);
		function(commandBuffer);
		commandBuffer->End();

		VkSubmitInfo submitInfo = {
			.sType              = VK_STRUCTURE_TYPE_SUBMIT_INFO,
			.commandBufferCount = 1,
			.pCommandBuffers    = commandBuffer->GetHandlePtr()
		};

		if (signalFence)
		{
			VK_CALL(vkQueueSubmit(mQueueHandle, 1, &submitInfo, signalFence->GetHandle()));
			signalFence->Wait(timeout);
		}
		else
		{
			VK_CALL(vkQueueSubmit(mQueueHandle, 1, &submitInfo, VK_NULL_HANDLE));
			WaitIdle();
		}
	}

}