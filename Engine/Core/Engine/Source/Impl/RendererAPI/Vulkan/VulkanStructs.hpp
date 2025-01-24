#pragma once

namespace Cosmic
{

	struct QueueFamilyIndices
	{
		int32 GraphicsFamily = -1;
		int32 PresentFamily  = -1;
	};

	struct SwapchainSupportDetails
	{
		VkSurfaceCapabilitiesKHR   Capabilities;
		Vector<VkSurfaceFormatKHR> Formats;
		Vector<VkPresentModeKHR>   PresentModes;
	};

}
