#pragma once
#include <iostream>
#include <sstream>

#include <cstdlib>
#include <assert.h>
#include <vector>
#include "Platform.h"
class Vk_PhysicalDevice {
public:
	VkInstance instance;
	VkPhysicalDevice physicalDevice;

	Vk_PhysicalDevice(VkInstance vkinstance);
	~Vk_PhysicalDevice();

	void creategpu();
};