#pragma once
#include "version.h"
#include "util/util.h"
#include <vector>

#define VK_NO_PROTOTYPES
#include "vulkan/vulkan.h"
#undef VK_NO_PROTOTYPES

namespace vk {
	struct ExtProps {
		VkExtensionProperties data;

		const char* name() const;
		u32 specVer() const;
	};

	struct LayerProps {
		VkLayerProperties data;

		const char* name() const;
		Version specVer() const;
		u32 implVer() const;
		const char* desc() const;
	};

	struct VulkanFns {
		PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr;
		PFN_vkEnumerateInstanceVersion vkEnumerateInstanceVersion;
		PFN_vkEnumerateInstanceExtensionProperties vkEnumerateInstanceExtensionProperties;
		PFN_vkEnumerateInstanceLayerProperties vkEnumerateInstanceLayerProperties;
		PFN_vkCreateInstance vkCreateInstance;
	};

	struct Vulkan
	{
		const void* module;
		VulkanFns fns;

		static Result<Ref<Vulkan>, OsString> create();

		Vulkan(const void* module, VulkanFns fns);
		Vulkan(const Vulkan&) = delete;
		Vulkan(Vulkan&&);
		~Vulkan() noexcept;

		Version enumerateInstanceVersion() const;
		Result<Vec<ExtProps>, VkResult> instanceExtProps() const;
		Result<Vec<LayerProps>, VkResult> instanceLayerProps() const;
	};
}
