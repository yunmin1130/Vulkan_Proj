#include "BUILD_OPTIONS.h"
#include "Shared.h"

#if BUILD_ENABLE_VULKAN_RUNTIME_DEBUG

void ErrorCheck(VkResult result)
{
	if (result < 0)
	{
		switch (result)
		{
		case VK_ERROR_OUT_OF_HOST_MEMORY:
			std::cout << "VK_ERROR_OUT_OF_HOST_MEMORY" << std::endl;
			break;
		case VK_ERROR_OUT_OF_DEVICE_MEMORY:
			break;
		case VK_ERROR_INITIALIZATION_FAILED:
			break;
		case VK_ERROR_DEVICE_LOST:
			break;
		default:
			break;
		}

		assert(0 && "Vulkan runtime error. ");
	}
}

uint32_t FindMemoryTypeIndex(const VkPhysicalDeviceMemoryProperties * gpu_memory_properties, const VkMemoryRequirements * memory_requirements, const VkMemoryPropertyFlags required_properties)
{
	//auto gpu_memroy_properties = _renderer->GetVulkanPhysicalDeviceMemoryProperties();
	//auto required_properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;

	for (uint32_t i = 0; i < gpu_memory_properties->memoryTypeCount; ++i)
	{
		if (memory_requirements->memoryTypeBits && (1 << i))
		{
			if ((gpu_memory_properties->memoryTypes[i].propertyFlags && required_properties) == required_properties)
			{
				return i;
			}
		}
	}
	assert(0 && "Couldn't find proper memory type.");
	return UINT32_MAX;
}

#else 

void ErrorCheck(VkResult result) {}; 

#endif // BUILD_ENABLE_VULKAN_RUNTIME_DEBUG 