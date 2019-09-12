#pragma once
#include "vk.h"

namespace vk {
	enum class InstanceCreateErr {
		HostOom = VK_ERROR_OUT_OF_HOST_MEMORY,
		DeviceOom = VK_ERROR_OUT_OF_DEVICE_MEMORY,
		InitFailed = VK_ERROR_INITIALIZATION_FAILED,
		LayerNotPresent = VK_ERROR_LAYER_NOT_PRESENT,
		ExtNotPresent = VK_ERROR_EXTENSION_NOT_PRESENT,
		IncompatibleDriver = VK_ERROR_INCOMPATIBLE_DRIVER,
	};

	struct Instance
	{
		Ref<Vulkan> lib;
		VkInstance vk;
		struct Fns {
			PFN_vkDestroyInstance vkDestroyInstance;
			PFN_vkEnumeratePhysicalDevices vkEnumeratePhysicalDevices;
		} fns;

		static Result<Instance, InstanceCreateErr> create(Ref<Vulkan> lib);

		Instance(Ref<Vulkan> lib, VkInstance vk, Fns fns);
		Instance(const Instance&) = delete;
		Instance(Instance&&);
		~Instance() noexcept;
	};
}
