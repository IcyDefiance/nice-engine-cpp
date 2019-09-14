#include "instance.h"

using namespace std;

#define LOAD_VK_INSTANCE_PFN(instance, name) \
	fns.##name = reinterpret_cast<PFN_##name>(lib->fns.vkGetInstanceProcAddr(instance, #name));

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
	Result<Ref<Instance>, InstanceCreateErr> Instance::create(Ref<Vulkan> lib) {
		VkInstanceCreateInfo createInfo;
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pNext = nullptr;
		createInfo.flags = 0;
		createInfo.pApplicationInfo = nullptr;
		createInfo.enabledLayerCount = 0;
		createInfo.ppEnabledLayerNames = nullptr;
		createInfo.enabledExtensionCount = 0;
		createInfo.ppEnabledExtensionNames = nullptr;

		// TODO: support allocation callbacks
		VkInstance vk;
		auto res = lib->fns.vkCreateInstance(&createInfo, nullptr, &vk);
		if (res != VK_SUCCESS) {
			return Err(static_cast<InstanceCreateErr>(res));
		}

		Fns fns;
		LOAD_VK_INSTANCE_PFN(vk, vkCreateDevice);
		LOAD_VK_INSTANCE_PFN(vk, vkDestroyInstance);
		LOAD_VK_INSTANCE_PFN(vk, vkEnumeratePhysicalDevices);
		LOAD_VK_INSTANCE_PFN(vk, vkGetDeviceProcAddr);

		return Ok(make_shared<Instance>(lib, vk, fns));
	}

	Instance::Instance(Ref<Vulkan> lib, VkInstance vk, Fns fns) : lib(lib), vk(vk), fns(fns) {}

	Instance::Instance(Instance&& other) : lib(other.lib), vk(other.vk), fns(other.fns) {
		other.vk = nullptr;
	}

	Instance::~Instance() noexcept {
		if (this->vk) {
			this->fns.vkDestroyInstance(this->vk, nullptr);
		}
	}
}
