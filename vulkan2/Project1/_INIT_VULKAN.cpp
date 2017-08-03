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
	instance = new vulkan_instance();
	gpu = new Vk_PhysicalDevice(instance->instance);
	device = new Vulkan_Device(gpu->physicalDevice);
}

vulkan_class::~vulkan_class()
{
}

Window * vulkan_class::OpenWindow(uint32_t size_x, uint32_t size_y, std::string name)
{
	window = new Window(this, size_x, size_y, name);
	return window;
}

const VkDevice vulkan_class::GetVulkanDevice() const
{
	return device->device;
}

const VkQueue vulkan_class::GetVulkanQueue() const
{
}

const uint32_t vulkan_class::GetVulkanGraphicsQueueFamilyIndex() const
{
}

const VkPhysicalDeviceProperties & vulkan_class::GetVulkanPhysicalDeviceProperties() const
{
}

const VkPhysicalDeviceMemoryProperties & vulkan_class::GetVulkanPhysicalDeviceMemoryProperties() const
{
}

void vulkan_class::InitVulkan()
{
	instance->createInstance();
	gpu->creategpu();
	device->createdevice();
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
}

const VkPhysicalDevice vulkan_class::GetVulkanPhysicalDevice() const
{
}
