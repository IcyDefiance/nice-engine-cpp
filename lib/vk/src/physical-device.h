#pragma once
#include "instance.h"

namespace vk {
	enum class PhysicalDeviceEnumErr {
		HostOom = VK_ERROR_OUT_OF_HOST_MEMORY,
		DeviceOom = VK_ERROR_OUT_OF_DEVICE_MEMORY,
		InitFailed = VK_ERROR_INITIALIZATION_FAILED
	};

	struct PhysicalDevice
	{
		const Instance& instance;
		VkPhysicalDevice vk;

		static Result<Vec<PhysicalDevice>, PhysicalDeviceEnumErr> enumerate(const Instance& instance);

		PhysicalDevice(const Instance& instance, VkPhysicalDevice vk);
	};
}
