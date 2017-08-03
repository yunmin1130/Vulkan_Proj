#pragma once
#include "Platform.h"

class Vulkan_Device
{
public:
	Vulkan_Device(VkPhysicalDevice physicaldevice);
	~Vulkan_Device();

	void createdevice();

	uint32_t GetQueueFamilyIndex(VkQueueFlags requestedFlag);

	VkDevice						device;
	VkPhysicalDevice				gpu;
	VkQueue							queue;

	VkPhysicalDeviceProperties physicalDeviceProperties = {};
	VkPhysicalDeviceFeatures physicalDeviceFeatures = {};
	VkPhysicalDeviceMemoryProperties physicalDeviceMemoryProperties = {};
	std::vector<VkQueueFamilyProperties> queueFamilyProperties{};

	struct {
		uint32_t graphic;
		uint32_t compute;
		uint32_t transfer;
	}queueFamilyindex;

	std::vector<const char*>	deviceExtensions;
};

