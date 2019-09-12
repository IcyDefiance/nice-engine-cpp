#include "physical-device.h"

namespace vk {
	// Success
	// * VK_SUCCESS
	// * VK_INCOMPLETE
	// Failure
	// * VK_ERROR_OUT_OF_HOST_MEMORY
	// * VK_ERROR_OUT_OF_DEVICE_MEMORY
	// * VK_ERROR_INITIALIZATION_FAILED
	Result<Vec<PhysicalDevice>, PhysicalDeviceEnumErr> PhysicalDevice::enumerate(const Instance& instance) {
		u32 size;
		auto res = instance.fns.vkEnumeratePhysicalDevices(instance.vk, &size, nullptr);
		if (res != VK_SUCCESS) {
			return Err(static_cast<PhysicalDeviceEnumErr>(res));
		}

		Vec<VkPhysicalDevice> pdevices(size);
		res = instance.fns.vkEnumeratePhysicalDevices(instance.vk, &size, pdevices.data());
		if (res != VK_SUCCESS) {
			return Err(static_cast<PhysicalDeviceEnumErr>(res));
		}

		Vec<PhysicalDevice> ret;
		ret.reserve(size);
		for (auto pdev : pdevices) {
			ret.push_back(PhysicalDevice(instance, pdev));
		}
		return Ok(ret);
	}

	PhysicalDevice::PhysicalDevice(const Instance& instance, VkPhysicalDevice vk) : instance(instance), vk(vk) {}
}
