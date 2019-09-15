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
		Ref<Instance> instance;
		VkPhysicalDevice vk;

		static auto enumerate(Ref<Instance> instance) {
			return enumerateImpl(instance).map([&](auto&& vks) {
				return iter(move(vks)) | Transform([&](auto vk) { return std::make_shared<PhysicalDevice>(instance, vk); });
			});
		}

		PhysicalDevice(Ref<Instance> instance, VkPhysicalDevice vk);

	private:
		static Result<Vec<VkPhysicalDevice>, PhysicalDeviceEnumErr> enumerateImpl(Ref<Instance> instance);
	};
}
