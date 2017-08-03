#include "vulkan_physicaldevice.h"

Vk_PhysicalDevice::Vk_PhysicalDevice(VkInstance vkinstance)
{
	instance = vkinstance;
}

Vk_PhysicalDevice::~Vk_PhysicalDevice()
{
}

void Vk_PhysicalDevice::creategpu()
{
	uint32_t gpuCount = 0;

	vkEnumeratePhysicalDevices(instance, &gpuCount, nullptr);
	std::vector<VkPhysicalDevice> gpuList(gpuCount);
	vkEnumeratePhysicalDevices(instance, &gpuCount, gpuList.data());

	physicalDevice = gpuList[0];
}
