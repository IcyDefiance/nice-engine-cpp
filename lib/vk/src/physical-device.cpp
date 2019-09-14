#include "physical-device.h"

namespace vk {
	// Success
	// * VK_SUCCESS
	// * VK_INCOMPLETE
	// Failure
	// * VK_ERROR_OUT_OF_HOST_MEMORY
	// * VK_ERROR_OUT_OF_DEVICE_MEMORY
	// * VK_ERROR_INITIALIZATION_FAILED
	Result<Vec<VkPhysicalDevice>, PhysicalDeviceEnumErr> PhysicalDevice::enumerateImpl(const Ref<Instance>& instance) {
		u32 size;
		auto res = instance->fns.vkEnumeratePhysicalDevices(instance->vk, &size, nullptr);
		if (res != VK_SUCCESS) {
			return Err(static_cast<PhysicalDeviceEnumErr>(res));
		}

		Vec<VkPhysicalDevice> ret(size);
		res = instance->fns.vkEnumeratePhysicalDevices(instance->vk, &size, ret.data());
		if (res != VK_SUCCESS) {
			return Err(static_cast<PhysicalDeviceEnumErr>(res));
		}

		return Ok(ret);
	}

	PhysicalDevice::PhysicalDevice(const Ref<Instance>& instance, VkPhysicalDevice vk) : instance(instance), vk(vk) {}
}
