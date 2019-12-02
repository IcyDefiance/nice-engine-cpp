#pragma once
#include "vk.h"

namespace vk {
	struct InstanceFns {
		PFN_vkCreateDevice vkCreateDevice;
		PFN_vkDestroyInstance vkDestroyInstance;
		PFN_vkEnumeratePhysicalDevices vkEnumeratePhysicalDevices;
		PFN_vkGetDeviceProcAddr vkGetDeviceProcAddr;
	};

	struct Instance
	{
		Ref<Vulkan> lib;
		VkInstance vk;
		InstanceFns fns;

		static Result<Ref<Instance>, VkResult> create(Ref<Vulkan> lib);

		Instance(Ref<Vulkan> lib, VkInstance vk, InstanceFns fns);
		Instance(const Instance&) = delete;
		Instance(Instance&&);
		~Instance() noexcept;
	};
}
