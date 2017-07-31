#include "_INIT_VULKAN.h"

#include "BUILD_OPTIONS.h"
#include "Platform.h"


//#include "Renderer.h"
#include "Shared.h"
#include "Window.h"


#include <cstdlib>
#include <assert.h>
#include <vector>
#include <iostream>
#include <sstream>


_INIT_VULKAN::_INIT_VULKAN()
{
	_INIT();
}


_INIT_VULKAN::~_INIT_VULKAN()
{
	_DEINIT();
}

void _INIT_VULKAN::_INIT()
{

		// --------------------- INSTANCE ---------------------------- //
		VkApplicationInfo application_info{};
		application_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		application_info.apiVersion = VK_MAKE_VERSION(1, 0, 2);
		application_info.applicationVersion = VK_MAKE_VERSION(0, 0, 4);
		application_info.pApplicationName = "Vulkan Tutorial 1";

		VkInstanceCreateInfo instance_create_info{};
		instance_create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		instance_create_info.pApplicationInfo = &application_info;
		instance_create_info.enabledLayerCount = _instance_layers.size();
		instance_create_info.ppEnabledLayerNames = _instance_layers.data();
		instance_create_info.enabledExtensionCount = _instance_extensions.size();
		instance_create_info.ppEnabledExtensionNames = _instance_extensions.data();
		instance_create_info.pNext = &debug_callback_create_info;

		ErrorCheck(vkCreateInstance(&instance_create_info, nullptr, &_instance));
		// --------------------- INSTANCE ---------------------------- //




		// --------------------- DEVICE ------------------------------ //
		{
			uint32_t gpu_count = 0;
			vkEnumeratePhysicalDevices(_instance, &gpu_count, nullptr);
			std::vector<VkPhysicalDevice> gpu_list(gpu_count);
			vkEnumeratePhysicalDevices(_instance, &gpu_count, gpu_list.data());
			_gpu = gpu_list[0];
			vkGetPhysicalDeviceProperties(_gpu, &_gpu_properties);
			vkGetPhysicalDeviceMemoryProperties(_gpu, &_gpu_memory_properties);
		}

		{
			uint32_t family_count = 0;
			vkGetPhysicalDeviceQueueFamilyProperties(_gpu, &family_count, nullptr);
			std::vector<VkQueueFamilyProperties> family_property_list(family_count);
			vkGetPhysicalDeviceQueueFamilyProperties(_gpu, &family_count, family_property_list.data());


			bool found = false;
			for (uint32_t i = 0; i < family_count; ++i)
			{
				if (family_property_list[i].queueFlags && VK_QUEUE_GRAPHICS_BIT)
				{
					found = true;
					_graphics_family_index = i;
				}
			}

			if (!found)
			{
				assert(0 && "Vulkan ERROR : Queue family supporting graphics not found.");
				std::exit(-1);
			}
		}

		{
			uint32_t layer_count = 0;
			vkEnumerateInstanceLayerProperties(&layer_count, nullptr);
			std::vector<VkLayerProperties> layer_property_list(layer_count);
			vkEnumerateInstanceLayerProperties(&layer_count, layer_property_list.data());
			std::cout << "Instance Layers : \n";
			for (auto &i : layer_property_list)
			{
				std::cout << " " << i.layerName << "\t\t | " << i.description << std::endl;
			}
			std::cout << std::endl;
		}

		{
			uint32_t layer_count = 0;
			vkEnumerateDeviceLayerProperties(_gpu, &layer_count, nullptr);
			std::vector<VkLayerProperties> layer_property_list(layer_count);
			vkEnumerateDeviceLayerProperties(_gpu, &layer_count, layer_property_list.data());
			std::cout << "Device Layers : \n";
			for (auto &i : layer_property_list)
			{
				std::cout << " " << i.layerName << "\t\t | " << i.description << std::endl;
			}
			std::cout << std::endl;
		}


		float queue_priorities[]{ 1.0f };
		VkDeviceQueueCreateInfo device_queue_create_info{};
		device_queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		device_queue_create_info.queueFamilyIndex = _graphics_family_index;
		device_queue_create_info.queueCount = 1;
		device_queue_create_info.pQueuePriorities = queue_priorities;

		VkDeviceCreateInfo device_create_info{};
		device_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		device_create_info.queueCreateInfoCount = 1;
		device_create_info.pQueueCreateInfos = &device_queue_create_info;
		device_create_info.enabledLayerCount = _device_layers.size();
		device_create_info.ppEnabledLayerNames = _device_layers.data();
		device_create_info.enabledExtensionCount = _device_extensions.size();
		device_create_info.ppEnabledExtensionNames = _device_extensions.data();

		ErrorCheck(vkCreateDevice(_gpu, &device_create_info, nullptr, &_device));


		vkGetDeviceQueue(_device, _graphics_family_index, 0, &_queue);
		// --------------------- DEVICE ------------------------------ //


	

}

void _INIT_VULKAN::_DEINIT()
{
	// ------------------- DESTROY DEVICE ------------------- //
	vkDestroyDevice(_device, nullptr);
	_device = nullptr;
	// ------------------- DESTROY DEVICE ------------------- //



	// ------------------- DESTROY INSTANCE ------------------- //
	vkDestroyInstance(_instance, nullptr);
	_instance = nullptr;
	// ------------------- DESTROY INSTANCE ------------------- //
}
