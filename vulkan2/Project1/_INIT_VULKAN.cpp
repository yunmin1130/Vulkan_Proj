#include "_INIT_VULKAN.h"
#include "Shared.h"
#include "Build_Options.h"
#include "Platform.h"
#include "Window.h"

#include <iostream>
#include <sstream>

#include <cstdlib>
#include <assert.h>
#include <vector>

vulkan_class::vulkan_class()
{
	SetupLayersAndExtenstions();
}

vulkan_class::~vulkan_class()
{
	DeInitDevice();
	DeInitInstance();
}

Window * vulkan_class::OpenWindow(uint32_t size_x, uint32_t size_y, std::string name)
{
	window = new Window(this, size_x, size_y, name);
	return window;
}

const VkDevice vulkan_class::GetVulkanDevice() const
{
	return device;
}

const VkQueue vulkan_class::GetVulkanQueue() const
{
	return queue;
}

const uint32_t vulkan_class::GetVulkanGraphicsQueueFamilyIndex() const
{
	return queueFamilyindex.graphic;
}

const VkPhysicalDeviceProperties & vulkan_class::GetVulkanPhysicalDeviceProperties() const
{
	return physicalDeviceProperties;
}

const VkPhysicalDeviceMemoryProperties & vulkan_class::GetVulkanPhysicalDeviceMemoryProperties() const
{
	return physicalDeviceMemoryProperties;
}

void vulkan_class::InitVulkan()
{
	InitInstance();
	InitPhysicalDevice();
	InitDevice();
}

void vulkan_class::InitInstance()
{
	VkApplicationInfo appInfo{};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "Vulkan Example";
	appInfo.apiVersion = VK_API_VERSION_1_0;

	VkInstanceCreateInfo instanceCreateInfo{};
	instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instanceCreateInfo.pNext = NULL;
	instanceCreateInfo.pApplicationInfo = &appInfo;
	instanceCreateInfo.enabledExtensionCount = instanceExtensions.size();
	instanceCreateInfo.ppEnabledExtensionNames = instanceExtensions.data();

	vkCreateInstance(&instanceCreateInfo, nullptr, &instance);
}

void vulkan_class::InitPhysicalDevice()
{
	uint32_t gpuCount = 0;

	vkEnumeratePhysicalDevices(instance, &gpuCount, nullptr);
	std::vector<VkPhysicalDevice> gpuList(gpuCount);
	vkEnumeratePhysicalDevices(instance, &gpuCount, gpuList.data());

	physicalDevice = gpuList[0];

	vkGetPhysicalDeviceProperties(physicalDevice, &physicalDeviceProperties);
	vkGetPhysicalDeviceFeatures(physicalDevice, &physicalDeviceFeatures);
	vkGetPhysicalDeviceMemoryProperties(physicalDevice, &physicalDeviceMemoryProperties);

	uint32_t queueFamilyCount = 0;

	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);
	queueFamilyProperties.resize(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilyProperties.data());

	
}

void vulkan_class::InitDevice()
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

	vkCreateDevice(physicalDevice, &deviceCreateInfo, nullptr, &device);

	vkGetDeviceQueue(device, queueFamilyindex.graphic, 0, &queue);
}


uint32_t vulkan_class::GetQueueFamilyIndex(VkQueueFlags requestedFlag)
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

void vulkan_class::SetupLayersAndExtenstions()
{
	instanceExtensions.push_back(VK_KHR_SURFACE_EXTENSION_NAME);
	instanceExtensions.push_back(PLATFORM_SURFACE_EXTENSION_NAME);

	deviceExtensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
}

void vulkan_class::DeInitInstance()
{
	vkDestroyInstance(instance, nullptr);
	instance = nullptr;
}

void vulkan_class::DeInitDevice()
{
	vkDestroyDevice(device, nullptr);
	device = nullptr;
}

bool vulkan_class::Run()
{
	if (nullptr != window) {
		return window->Update();
	}
	return true;
}

const VkInstance vulkan_class::GetVulkanInstance() const
{
	return instance;
}

const VkPhysicalDevice vulkan_class::GetVulkanPhysicalDevice() const
{
	return physicalDevice;
}
