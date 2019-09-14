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
		Ref<Instance> instance;
		VkDevice vk;
		struct Fns {
			PFN_vkDestroyDevice vkDestroyDevice;
		} fns;

		static Result<Device, DeviceCreateErr> create(const PhysicalDevice& pdev);

		Device(Ref<Instance> instance, VkPhysicalDevice pdev, Fns fns);
		Device(const Device&) = delete;
		Device(Device&&);
		~Device() noexcept;
	};
}
