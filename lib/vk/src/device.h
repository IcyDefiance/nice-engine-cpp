#pragma once
#include "physical-device.h"

namespace vk {
	enum class DeviceCreateErr {
		HostOom = VK_ERROR_OUT_OF_HOST_MEMORY,
		DeviceOom = VK_ERROR_OUT_OF_DEVICE_MEMORY,
		InitFailed = VK_ERROR_INITIALIZATION_FAILED,
		ExtNotPresent = VK_ERROR_EXTENSION_NOT_PRESENT,
		FeatNotPresent = VK_ERROR_FEATURE_NOT_PRESENT,
		TooManyObjects = VK_ERROR_TOO_MANY_OBJECTS,
		DeviceLost = VK_ERROR_DEVICE_LOST,
	};

	struct Device
	{
		VkDevice vk;
		Ref<PhysicalDevice> pdev;
		struct Fns {
			PFN_vkDestroyDevice vkDestroyDevice;
			PFN_vkGetDeviceProcAddr vkGetDeviceProcAddr;
		} fns;

		static Result<Device, DeviceCreateErr> create(Ref<PhysicalDevice> pdev);

		Device(Ref<PhysicalDevice> pdev, VkDevice vk, Fns fns);
		Device(const Device&) = delete;
		Device(Device&&);
		~Device() noexcept;
	};
}
