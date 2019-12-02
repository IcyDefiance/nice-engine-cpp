#pragma once
#include "physical-device.h"

namespace vk {
	struct DeviceFns {
		PFN_vkDestroyDevice vkDestroyDevice;
		PFN_vkGetDeviceProcAddr vkGetDeviceProcAddr;
	};

	struct Device
	{
		VkDevice vk;
		Ref<PhysicalDevice> pdev;
		DeviceFns fns;

		static Result<Device, VkResult> create(Ref<PhysicalDevice> pdev);

		Device(Ref<PhysicalDevice> pdev, VkDevice vk, DeviceFns fns);
		Device(const Device&) = delete;
		Device(Device&&);
		~Device() noexcept;
	};
}
