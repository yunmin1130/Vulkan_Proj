#pragma once

#include "Platform.h"
#include <vector>

//class Window;
#include "Window.h"
#include "_INIT_VULKAN.h"

class Renderer
{
public:
	Renderer();
	~Renderer();
	 
	Window *OpenWindow(uint32_t size_x, uint32_t size_y, std::string name);

	bool Run();

	const VkInstance GetVulkanInstance() const;
	const VkPhysicalDevice GetVulkanPhysicalDevice() const;
	const VkDevice GetVulkanDevice() const;
	const VkQueue GetVulkanQueue() const;
	const uint32_t GetVulkanGraphicsQueueFamilyIndex() const;
	const VkPhysicalDeviceProperties &GetVulkanPhysicalDeviceProperties() const;
	const VkPhysicalDeviceMemoryProperties &GetVulkanPhysicalDeviceMemoryProperties() const;


//private:
	void _SetupLayersAndExtensions();

	void _InitInstance();
	void _DeInitInstance();

	void _InitDevice();
	void _DeInitDevice();

	void _INIT();
	void _DEINIT();


	void _SetupDebug();
	void _InitDebug();
	void _DeInitDebug();

	
	VkInstance _instance = VK_NULL_HANDLE;
	VkPhysicalDevice _gpu = VK_NULL_HANDLE;
	VkDevice _device = VK_NULL_HANDLE;
	VkQueue _queue = VK_NULL_HANDLE;
	VkPhysicalDeviceProperties _gpu_properties = {};
	VkPhysicalDeviceMemoryProperties _gpu_memory_properties = {}; 
	


	uint32_t _graphics_family_index = 0;

	Window *_window = nullptr;

	
	std::vector<const char*> _instance_layers;
	std::vector<const char*> _instance_extensions;
	std::vector<const char*> _device_layers;
	std::vector<const char*> _device_extensions;
	

	VkDebugReportCallbackEXT _debug_report = VK_NULL_HANDLE;
	VkDebugReportCallbackCreateInfoEXT debug_callback_create_info = {};

};

