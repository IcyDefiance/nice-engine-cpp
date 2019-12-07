#include "gfx.h"
#include "fmt/core.h"
#include "SDL.h"
#include "SDL_vulkan.h"

using namespace std;

VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE;

vk::UniqueInstance createInstance(const char* applicationName, u32 applicationVersion) {
	auto applicationInfo = vk::ApplicationInfo()
		.setPApplicationName(applicationName)
		.setApplicationVersion(applicationVersion)
		.setApiVersion(VK_API_VERSION_1_1);

#ifdef NDEBUG
	Vec layers;
	Vec exts;
#else
	Set<string> prefLayers {"VK_LAYER_LUNARG_standard_validation"};
	auto layerProps = vk::enumerateInstanceLayerProperties();
	auto layerNames = iter(layerProps)
		| Transform([&](auto&& x) { return x.get().layerName; });
	auto layers = prefLayers.intersection(move(layerNames))
		| ToVec();

	Vec exts {"VK_EXT_debug_utils"};
#endif


	auto ci = vk::InstanceCreateInfo()
		.setPApplicationInfo(&applicationInfo)
		.setEnabledLayerCount(static_cast<u32>(layers.size()))
		.setPpEnabledLayerNames(layers.data())
		.setEnabledExtensionCount(static_cast<u32>(exts.size()))
		.setPpEnabledExtensionNames(exts.data());

	auto ret = vk::createInstanceUnique(ci);
	VULKAN_HPP_DEFAULT_DISPATCHER.init(*ret);
	return ret;
}

VkBool32 debugUtilsMessengerCallback(
	VkDebugUtilsMessageSeverityFlagBitsEXT vkMessageSeverity,
	VkDebugUtilsMessageTypeFlagsEXT vkMessageTypes,
	const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
	void* pUserData
) {
	auto messageSeverity = static_cast<vk::DebugUtilsMessageSeverityFlagBitsEXT>(vkMessageSeverity);
	vk::DebugUtilsMessageTypeFlagsEXT messageTypes(vkMessageTypes);
	fmt::print("{} {} {}\n", vk::to_string(messageSeverity), vk::to_string(messageTypes), pCallbackData->pMessage);
	return VK_FALSE;
}

#ifndef NDEBUG
vk::UniqueDebugUtilsMessengerEXT createDebugMessenger(vk::Instance instance) {
	auto ci = vk::DebugUtilsMessengerCreateInfoEXT()
		.setMessageSeverity(
			vk::DebugUtilsMessageSeverityFlagBitsEXT::eError
			| vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning
			| vk::DebugUtilsMessageSeverityFlagBitsEXT::eInfo
			| vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose
		)
		.setMessageType(
			vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral
			| vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation
			| vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance
		)
		.setPfnUserCallback(debugUtilsMessengerCallback);

	return instance.createDebugUtilsMessengerEXTUnique(ci);
}
#endif

pair<vk::UniqueDevice, vk::Queue> createDevice(vk::Instance instance) {
	auto pdevice = instance.enumeratePhysicalDevices()[0];

	auto qfam = (iter(pdevice.getQueueFamilyProperties())
		| Enumerate()
		| Filter([](auto&& pair) { return pair.second.queueFlags & vk::QueueFlagBits::eGraphics; })
		| Transform([](auto&& pair) { return pair.first; })
		| First())
		.unwrap();

	f32 priority = 1;
	auto qci = vk::DeviceQueueCreateInfo()
		.setQueueFamilyIndex(qfam)
		.setQueueCount(1)
		.setPQueuePriorities(&priority);

	auto ci = vk::DeviceCreateInfo()
		.setQueueCreateInfoCount(1)
		.setPQueueCreateInfos(&qci);

	auto device = pdevice.createDeviceUnique(ci);
	auto queue = device->getQueue(qfam, 0);
	return pair(move(device), queue);
}

Result<Ref<Gfx>, const char*> Gfx::create(const char* name, u32 version) {
	if (SDL_Init(SDL_INIT_VIDEO) || SDL_Vulkan_LoadLibrary(nullptr)) {
		return Err(SDL_GetError());
	}
	VULKAN_HPP_DEFAULT_DISPATCHER.init(reinterpret_cast<PFN_vkGetInstanceProcAddr>(SDL_Vulkan_GetVkGetInstanceProcAddr()));

	auto instance = createInstance(name, version);
#ifndef NDEBUG
	auto debugMessenger = createDebugMessenger(*instance);
#endif

	auto [device, queue] = createDevice(*instance);

	return Ok(Ref<Gfx>(new Gfx {
		move(instance),
#ifndef NDEBUG
		move(debugMessenger),
#endif
		move(device),
		queue,
	}));
}
