#pragma once
#include "Platform.h"

#include <vector>

class Window;

class vulkan_class
{
private:

	void SetupLayersAndExtenstions();

	void InitInstance();
	void DeInitInstance();

	void InitPhysicalDevice();

	uint32_t GetQueueFamilyIndex(VkQueueFlags requestedFlag);

	void InitDevice();
	void DeInitDevice();

public:
	vulkan_class();
	~vulkan_class();

	void InitVulkan();

	Window				* OpenWindow(uint32_t size_x, uint32_t size_y, std::string name);

	bool				Run();

	const VkInstance						GetVulkanInstance() const;
	const VkPhysicalDevice					GetVulkanPhysicalDevice() const;
	const VkDevice							GetVulkanDevice() const;
	const VkQueue							GetVulkanQueue() const;
	const uint32_t							GetVulkanGraphicsQueueFamilyIndex() const;
	const VkPhysicalDeviceProperties		&GetVulkanPhysicalDeviceProperties() const;
	const VkPhysicalDeviceMemoryProperties	&GetVulkanPhysicalDeviceMemoryProperties() const;

	VkInstance			instance = VK_NULL_HANDLE;
	VkPhysicalDevice	physicalDevice = VK_NULL_HANDLE;
	VkDevice			device = VK_NULL_HANDLE;
	VkQueue				queue = VK_NULL_HANDLE;
	VkPhysicalDeviceProperties physicalDeviceProperties = {};
	VkPhysicalDeviceFeatures physicalDeviceFeatures = {};
	VkPhysicalDeviceMemoryProperties physicalDeviceMemoryProperties = {};
	std::vector<VkQueueFamilyProperties> queueFamilyProperties{};

	struct{
		uint32_t graphic;
		uint32_t compute;
		uint32_t transfer;
	}queueFamilyindex;

	Window				*window = nullptr;

	//std::vector<const char*>	instance_layers;
	std::vector<const char*>	instanceExtensions;
	//std::vector<const char*>	device_layers;
	std::vector<const char*>	deviceExtensions;
};