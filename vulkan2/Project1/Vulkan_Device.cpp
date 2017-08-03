#include "Vulkan_Device.h"

Vulkan_Device::Vulkan_Device(VkPhysicalDevice physicaldevice)
{
	gpu = physicaldevice;

	deviceExtensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
}

Vulkan_Device::~Vulkan_Device()
{
	vkDestroyDevice(device, nullptr);
	device = nullptr;
}

void Vulkan_Device::createdevice()
{
	std::vector<VkDeviceQueueCreateInfo> queueCreateInfo{};

	float queuePriority(1.0f);

	VkQueueFlags queueFlag = VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT;

	if (queueFlag & VK_QUEUE_GRAPHICS_BIT) {
		queueFamilyindex.graphic = GetQueueFamilyIndex(VK_QUEUE_GRAPHICS_BIT);
		VkDeviceQueueCreateInfo queueInfo{};
		queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueInfo.queueFamilyIndex = queueFamilyindex.graphic;
		queueInfo.queueCount = 1;
		queueInfo.pQueuePriorities = &queuePriority;
		queueCreateInfo.push_back(queueInfo);
	}
	else {
		queueFamilyindex.graphic = VK_NULL_HANDLE;
	}

	if (queueFlag & VK_QUEUE_COMPUTE_BIT) {
		queueFamilyindex.compute = GetQueueFamilyIndex(VK_QUEUE_COMPUTE_BIT);
		VkDeviceQueueCreateInfo queueInfo{};
		queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueInfo.queueFamilyIndex = queueFamilyindex.compute;
		queueInfo.queueCount = 1;
		queueInfo.pQueuePriorities = &queuePriority;
		queueCreateInfo.push_back(queueInfo);
	}
	else {
		queueFamilyindex.compute = queueFamilyindex.graphic;
	}

	if (queueFlag & VK_QUEUE_TRANSFER_BIT) {
		queueFamilyindex.transfer = GetQueueFamilyIndex(VK_QUEUE_TRANSFER_BIT);
		VkDeviceQueueCreateInfo queueInfo{};
		queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueInfo.queueFamilyIndex = queueFamilyindex.transfer;
		queueInfo.queueCount = 1;
		queueInfo.pQueuePriorities = &queuePriority;
		queueCreateInfo.push_back(queueInfo);
	}
	else {
		queueFamilyindex.transfer = queueFamilyindex.graphic;
	}

	VkDeviceCreateInfo deviceCreateInfo = {};
	deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	deviceCreateInfo.queueCreateInfoCount = (uint32_t)queueCreateInfo.size();
	deviceCreateInfo.pQueueCreateInfos = queueCreateInfo.data();
	deviceCreateInfo.enabledExtensionCount = (uint32_t)deviceExtensions.size();
	deviceCreateInfo.ppEnabledExtensionNames = deviceExtensions.data();

	vkCreateDevice(gpu, &deviceCreateInfo, nullptr, &device);

	vkGetDeviceQueue(device, queueFamilyindex.graphic, 0, &queue);
}

uint32_t Vulkan_Device::GetQueueFamilyIndex(VkQueueFlags requestedFlag)
{
	if (requestedFlag & VK_QUEUE_COMPUTE_BIT) {
		for (uint32_t i = 0; i < queueFamilyProperties.size(); i++) {
			if ((queueFamilyProperties[i].queueFlags & requestedFlag) && ((queueFamilyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) == 0)) {
				return i;
				break;
			}
		}
	}

	if (requestedFlag & VK_QUEUE_TRANSFER_BIT) {
		for (uint32_t i = 0; i < queueFamilyProperties.size(); i++) {
			if ((queueFamilyProperties[i].queueFlags & requestedFlag) && ((queueFamilyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) == 0) && ((queueFamilyProperties[i].queueFlags & VK_QUEUE_COMPUTE_BIT) == 0)) {
				return i;
				break;
			}
		}
	}


	for (uint32_t i = 0; i < queueFamilyProperties.size(); i++) {
		if (queueFamilyProperties[i].queueFlags & requestedFlag) {
			return i;
			break;
		}
	}
}
