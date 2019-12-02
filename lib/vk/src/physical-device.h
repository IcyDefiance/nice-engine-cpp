#pragma once
#include "instance.h"

namespace vk {
	struct PhysicalDevice
	{
		Ref<Instance> instance;
		VkPhysicalDevice vk;

		static auto enumerate(Ref<Instance> instance) {
			return enumerateImpl(instance).map([&](auto&& vks) {
				return iter(move(vks))
					| Transform([&](auto vk) { return std::make_shared<PhysicalDevice>(instance, vk); });
			});
		}

		PhysicalDevice(Ref<Instance> instance, VkPhysicalDevice vk);

	private:
		static Result<Vec<VkPhysicalDevice>, VkResult> enumerateImpl(Ref<Instance> instance);
	};
}
