#include "Shared.h"
#include "Build_Options.h"

#if BUILD_ENABLE_VULKAN_RUNTIME_DEBUG
void ErrorCheck(VkResult result)
{
	if (result < 0) {
		switch (result) {
		case VK_ERROR_OUT_OF_HOST_MEMORY:
			std::cout << "VK_ERROR_OUT_OF_HOST_MEMORY" << std::endl;
			break;
		case VK_ERROR_OUT_OF_DEVICE_MEMORY:
			std::cout << "VK_ERROR_OUT_OF_DEVICE_MEMORY" << std::endl;
			break;
		case VK_ERROR_INITIALIZATION_FAILED:
			std::cout << "VK_ERROR_INITIALIZATION_FAILED" << std::endl;
			break;
		case VK_ERROR_DEVICE_LOST:
			std::cout << "VK_ERROR_DEVICE_LOST" << std::endl;
			break;
		case VK_ERROR_MEMORY_MAP_FAILED:
			std::cout << "VK_ERROR_MEMORY_MAP_FAILED" << std::endl;
			break;
		case VK_ERROR_LAYER_NOT_PRESENT:
			std::cout << "VK_ERROR_LAYER_NOT_PRESENT" << std::endl;
			break;
		case VK_ERROR_EXTENSION_NOT_PRESENT:
			std::cout << "VK_ERROR_EXTENSION_NOT_PRESENT" << std::endl;
			break;
		case VK_ERROR_FEATURE_NOT_PRESENT:
			std::cout << "VK_ERROR_FEATURE_NOT_PRESENT" << std::endl;
			break;
		case VK_ERROR_INCOMPATIBLE_DRIVER:
			std::cout << "VK_ERROR_INCOMPATIBLE_DRIVER" << std::endl;
			break;
		case VK_ERROR_TOO_MANY_OBJECTS:
			std::cout << "VK_ERROR_TOO_MANY_OBJECTS" << std::endl;
			break;
		case VK_ERROR_FORMAT_NOT_SUPPORTED:
			std::cout << "VK_ERROR_FORMAT_NOT_SUPPORTED" << std::endl;
			break;
		case VK_ERROR_FRAGMENTED_POOL:
			std::cout << "VK_ERROR_FRAGMENTED_POOL" << std::endl;
			break;
		case VK_ERROR_SURFACE_LOST_KHR:
			std::cout << "VK_ERROR_SURFACE_LOST_KHR" << std::endl;
			break;
		case VK_ERROR_NATIVE_WINDOW_IN_USE_KHR:
			std::cout << "VK_ERROR_NATIVE_WINDOW_IN_USE_KHR" << std::endl;
			break;
		case VK_SUBOPTIMAL_KHR:
			std::cout << "VK_SUBOPTIMAL_KHR" << std::endl;
			break;
		case VK_ERROR_OUT_OF_DATE_KHR:
			std::cout << "VK_ERROR_OUT_OF_DATE_KHR" << std::endl;
			break;
		case VK_ERROR_INCOMPATIBLE_DISPLAY_KHR:
			std::cout << "VK_ERROR_INCOMPATIBLE_DISPLAY_KHR" << std::endl;
			break;
		case VK_ERROR_VALIDATION_FAILED_EXT:
			std::cout << "VK_ERROR_VALIDATION_FAILED_EXT" << std::endl;
			break;
		case VK_ERROR_INVALID_SHADER_NV:
			std::cout << "VK_ERROR_INVALID_SHADER_NV" << std::endl;
			break;
		case VK_ERROR_OUT_OF_POOL_MEMORY_KHR:
			std::cout << "VK_ERROR_OUT_OF_POOL_MEMORY_KHR" << std::endl;
			break;
		case VK_ERROR_INVALID_EXTERNAL_HANDLE_KHR:
			std::cout << "VK_ERROR_INVALID_EXTERNAL_HANDLE_KHR" << std::endl;
			break;
		default:
			break;
		}
		assert(0 && "Vulkan rentime error.");
	}
}

uint32_t FindMemoryTypeIndex(const VkPhysicalDeviceMemoryProperties * gpu_memory_properties, const VkMemoryRequirements * memory_requirements, const VkMemoryPropertyFlags required_properties)
{
	for (uint32_t i = 0; i < gpu_memory_properties->memoryTypeCount; ++i) {
		if (memory_requirements->memoryTypeBits & (1 << i)) {
			if ((gpu_memory_properties->memoryTypes[i].propertyFlags & required_properties) == required_properties) {
				return i;
			}
		}
	}
	assert(0 && "Couldn't find proper memory type.");
	return UINT32_MAX;
}

#else

void ErrorCheck(VkResult result) {}

#endif // BUILD_ENABLE_VULKAN_RUNTIME_DEBUG