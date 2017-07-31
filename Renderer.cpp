#include "BUILD_OPTIONS.h"
#include "Platform.h"
#include "_INIT_VULKAN.h"

#include "Renderer.h"
#include "Shared.h"
#include "Window.h"


#include <cstdlib>
#include <assert.h>
#include <vector>
#include <iostream>
#include <sstream>


Renderer::Renderer()
{
	_SetupLayersAndExtensions();
	_SetupDebug();
	//_InitInstance();
	//_INIT();
	_InitDebug();
	//_InitDevice();
}


Renderer::~Renderer()
{
	delete _window;

	//_DEINIT();
 	//_DeInitDevice();
	_DeInitDebug();
	//_DeInitInstance();
}

Window * Renderer::OpenWindow(uint32_t size_x, uint32_t size_y, std::string name)
{
	_window = new Window(this, size_x, size_y, name);
	return _window;
}

bool Renderer::Run()
{
	if (_window != nullptr)
	{
		return _window->Update();
	}
	return true;
}

const VkInstance Renderer::GetVulkanInstance() const
{
	return _instance;
}

const VkPhysicalDevice Renderer::GetVulkanPhysicalDevice() const
{
	return _gpu;
}

const VkDevice Renderer::GetVulkanDevice() const
{
	return _device;
}

const VkQueue Renderer::GetVulkanQueue() const
{
	return _queue;
}

const uint32_t Renderer::GetVulkanGraphicsQueueFamilyIndex() const
{
	return _graphics_family_index;
}

const VkPhysicalDeviceProperties & Renderer::GetVulkanPhysicalDeviceProperties() const
{
	return _gpu_properties;
}

const VkPhysicalDeviceMemoryProperties & Renderer::GetVulkanPhysicalDeviceMemoryProperties() const
{
	return _gpu_memory_properties;
}

void Renderer::_SetupLayersAndExtensions()
{
	//_instance_extensions.push_back(VK_KHR_DISPLAY_EXTENSION_NAME);
	_instance_extensions.push_back(VK_KHR_SURFACE_EXTENSION_NAME);
	_instance_extensions.push_back(PLATFORM_SURFACE_EXTENSION_NAME);

	_device_extensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
}

/*
void Renderer::_InitInstance()
{
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

	ErrorCheck(vkCreateInstance(&instance_create_info, nullptr, &_instance ));
}
*/

/*
void Renderer::_DeInitInstance()
{
	vkDestroyInstance(_instance, nullptr);
	_instance = nullptr;
}
*/

/*
void Renderer::_InitDevice()
{
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

	ErrorCheck(vkCreateDevice(_gpu, &device_create_info , nullptr, &_device));

	vkGetDeviceQueue(_device, _graphics_family_index, 0, &_queue);
}
*/

/*
void Renderer::_DeInitDevice()
{
	vkDestroyDevice(_device, nullptr);
	_device = nullptr;
}
*/

/*
void Renderer::_INIT()
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

void Renderer::_DEINIT()
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
*/

#if BUILD_ENABLE_VULKAN_DEBUG

VKAPI_ATTR VkBool32 VKAPI_CALL
VulkanDebugCallback(
	VkDebugReportFlagsEXT flags,
	VkDebugReportObjectTypeEXT obj_type,
	uint64_t src_obj,
	size_t location,
	int32_t msg_code,
	const char* layer_prefix,
	const char* msg,
	void* user_data
)
{
	std::ostringstream stream;
	stream << "VKDBG : ";
	if (flags & VK_DEBUG_REPORT_INFORMATION_BIT_EXT)
	{
		stream << "INFO : ";
	}
	if (flags & VK_DEBUG_REPORT_WARNING_BIT_EXT)
	{
		stream << "WARNING : ";
	}	
	if (flags & VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT)
	{
		stream << "PERFORMANCE : ";
	}	
	if (flags & VK_DEBUG_REPORT_ERROR_BIT_EXT)
	{
		stream << "ERROR : ";
	}	
	if (flags & VK_DEBUG_REPORT_DEBUG_BIT_EXT)
	{
		stream << "DEBUG : ";
	}

	stream << "@[" << layer_prefix << "]: ";
	stream << msg << std::endl;
	std::cout << stream.str();

#ifdef _WIN32
	if (flags & VK_DEBUG_REPORT_ERROR_BIT_EXT)
	{
		MessageBox(NULL, stream.str().c_str(), "Vulkan Error!", 0);
	}
#endif
	return false;
}

void Renderer::_SetupDebug()
{
	debug_callback_create_info.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CREATE_INFO_EXT;
	debug_callback_create_info.pfnCallback = VulkanDebugCallback;
	debug_callback_create_info.flags =
		//VK_DEBUG_REPORT_INFORMATION_BIT_EXT |
		VK_DEBUG_REPORT_WARNING_BIT_EXT |
		VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT |
		VK_DEBUG_REPORT_ERROR_BIT_EXT |
		//VK_DEBUG_REPORT_DEBUG_BIT_EXT |
		VK_DEBUG_REPORT_FLAG_BITS_MAX_ENUM_EXT |
		0;

	_instance_layers.push_back("VK_LAYER_LUNARG_standard_validation");
	//_instance_layers.push_back("VK_LAYER_LUNARG_threading");
	//_instance_layers.push_back("VK_LAYER_LUNARG_draw_state");
	//_instance_layers.push_back("VK_LAYER_LUNARG_image");
	//_instance_layers.push_back("VK_LAYER_LUNARG_mem_tracker");
	//_instance_layers.push_back("VK_LAYER_LUNARG_object_tracker");
	//_instance_layers.push_back("VK_LAYER_LUNARG_param_checker");

	_instance_extensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);

	_device_layers.push_back("VK_LAYER_LUNARG_standard_validation");
	/*_device_layers.push_back("VK_LAYER_LUNARG_threading");
	_device_layers.push_back("VK_LAYER_LUNARG_draw_state");
	_device_layers.push_back("VK_LAYER_LUNARG_image");
	_device_layers.push_back("VK_LAYER_LUNARG_mem_tracker");
	_device_layers.push_back("VK_LAYER_LUNARG_object_tracker");
	_device_layers.push_back("VK_LAYER_LUNARG_param_checker");*/
}

PFN_vkCreateDebugReportCallbackEXT fvkCreateDebugReportCallbackEXT = nullptr;
PFN_vkDestroyDebugReportCallbackEXT fvkDestroyDebugReportCallbackEXT = nullptr;

void Renderer::_InitDebug()
{
	fvkCreateDebugReportCallbackEXT = (PFN_vkCreateDebugReportCallbackEXT)vkGetInstanceProcAddr(_instance, "vkCreateDebugReportCallbackEXT");
	fvkDestroyDebugReportCallbackEXT = (PFN_vkDestroyDebugReportCallbackEXT)vkGetInstanceProcAddr(_instance, "vkDestroyDebugReportCallbackEXT");
	if (fvkCreateDebugReportCallbackEXT == nullptr || fvkDestroyDebugReportCallbackEXT == nullptr)
	{
		assert(0 && "Vulkan ERROR : Can't fetch debug function pointers .");
		std::exit(-1);
	}

	

	fvkCreateDebugReportCallbackEXT(_instance, &debug_callback_create_info, nullptr, &_debug_report);
	//vkCreateDebugReportCallbackEXT(_instance, nullptr, nullptr, nullptr);
}

void Renderer::_DeInitDebug()
{
	fvkDestroyDebugReportCallbackEXT(_instance, _debug_report, nullptr);
	_debug_report = VK_NULL_HANDLE;
}


#else

void Renderer::_SetupDebug() {};
void Renderer::_InitDebug() {};
void Renderer::_DeInitDebug() {};

#endif	// BUILD_ENABLE_VULKAN_DEBUG 