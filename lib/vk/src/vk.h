#pragma once
#include "version.h"
#include "util/util.h"
#include <string>
#include <vector>

#define VK_NO_PROTOTYPES
#include "vulkan/vulkan.h"
#undef VK_NO_PROTOTYPES

namespace vk {
	struct ExtProps {
		Str name() const;
		u32 specVer() const;

	private:
		char extName[256];
		u32 specVersion;
	};
	static_assert(sizeof(ExtProps) == sizeof(VkExtensionProperties));

	struct LayerProps {
		Str name() const;
		Version specVer() const;
		u32 implVer() const;
		Str desc() const;

	private:
		char layerName[256];
		u32 specVersion;
		u32 implVersion;
		char description[256];
	};

	enum class InstanceExtPropsError {
		OutOfHostMemory = VK_ERROR_OUT_OF_HOST_MEMORY,
		OutOfDeviceMemory = VK_ERROR_OUT_OF_DEVICE_MEMORY,
		LayerNotPresent = VK_ERROR_LAYER_NOT_PRESENT,
	};

	enum class InstanceLayerPropsError {
		OutOfHostMemory = VK_ERROR_OUT_OF_HOST_MEMORY,
		OutOfDeviceMemory = VK_ERROR_OUT_OF_DEVICE_MEMORY,
	};

	struct Vulkan
	{
		const void* module;
		struct Fns {
			PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr;
			PFN_vkEnumerateInstanceVersion vkEnumerateInstanceVersion;
			PFN_vkEnumerateInstanceExtensionProperties vkEnumerateInstanceExtensionProperties;
			PFN_vkEnumerateInstanceLayerProperties vkEnumerateInstanceLayerProperties;
			PFN_vkCreateInstance vkCreateInstance;
		} fns;

		static Result<Ref<Vulkan>, OsString> create();

		Vulkan(const void* module, Fns fns);
		Vulkan(const Vulkan&) = delete;
		Vulkan(Vulkan&&);
		~Vulkan() noexcept;

		Version enumerateInstanceVersion() const;
		Result<Vec<ExtProps>, InstanceExtPropsError> instanceExtProps() const;
		Result<Vec<LayerProps>, InstanceLayerPropsError> instanceLayerProps() const;
	};
}
