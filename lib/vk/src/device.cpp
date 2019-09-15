#include "device.h"

#define LOAD_VK_DEVICE_PFN(device, name) \
	fns.##name = reinterpret_cast<PFN_##name>(instance->fns.vkGetDeviceProcAddr(device, #name));

namespace vk {
	// Success
	// * VK_SUCCESS
	// Failure
	// * VK_ERROR_OUT_OF_HOST_MEMORY
	// * VK_ERROR_OUT_OF_DEVICE_MEMORY
	// * VK_ERROR_INITIALIZATION_FAILED
	// * VK_ERROR_LAYER_NOT_PRESENT
	// * VK_ERROR_EXTENSION_NOT_PRESENT
	// * VK_ERROR_INCOMPATIBLE_DRIVER
	Result<Device, DeviceCreateErr> Device::create(const PhysicalDevice& pdev) {
		auto instance = pdev.instance;

		VkDeviceCreateInfo createInfo;
		createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		createInfo.pNext = nullptr;
		createInfo.flags = 0;
		createInfo.queueCreateInfoCount = 0;
		createInfo.pQueueCreateInfos = nullptr;
		createInfo.enabledLayerCount = 0;
		createInfo.ppEnabledLayerNames = nullptr;
		createInfo.enabledExtensionCount = 0;
		createInfo.ppEnabledExtensionNames = nullptr;
		createInfo.pEnabledFeatures = nullptr;

		// TODO: support allocation callbacks
		VkDevice vk;
		auto res = instance->fns.vkCreateDevice(pdev.vk, &createInfo, nullptr, &vk);
		if (res != VK_SUCCESS) {
			return Err(static_cast<DeviceCreateErr>(res));
		}

		Fns fns;
		LOAD_VK_DEVICE_PFN(vk, vkDestroyDevice);

		return Ok(Device(instance, vk, pdev.vk, fns));
	}

	Device::Device(Ref<Instance> instance, VkDevice vk, VkPhysicalDevice pdev, Fns fns) : instance(instance), vk(vk), pdev(pdev), fns(fns) {}

	Device::Device(Device&& other) : instance(other.instance), vk(other.vk), fns(other.fns) {
		other.vk = nullptr;
	}

	Device::~Device() noexcept {
		if (this->vk) {
			this->fns.vkDestroyDevice(this->vk, nullptr);
		}
	}
}
