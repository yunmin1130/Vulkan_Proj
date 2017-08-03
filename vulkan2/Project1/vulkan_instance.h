#pragma once
#include "Platform.h"
#include <vector>

class vulkan_instance
{
public:
	vulkan_instance();
	~vulkan_instance();

	VkInstance instance;

	//VulkanLayerAndExtension		layerExtension;

	std::vector<const char*>	instanceExtensions;

	VkResult createInstance(std::vector<const char *>& layers, std::vector<const char *>& extensionNames, const char* applicationName);
	void destroyInstance();
	
};

